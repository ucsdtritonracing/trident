#include "platform/can/can_stm32.h"

#include "platform/can/can.h"

#include <stm32g4xx_hal.h>

#include <cstring>

namespace Platform::Can {

namespace {

// The HAL copies up to 64 bytes into the receive buffer, the CAN FD maximum.
constexpr int MAX_FDCAN_DATA_LENGTH = 64;

} // namespace

SendStatus Stm32Peripheral::Send(const Message& message) {
    const uint32_t max_id = message.extended ? MAX_EXTENDED_ID : MAX_STANDARD_ID;
    if (message.length > MAX_CAN_DATA_LENGTH || message.id > max_id) {
        return SendStatus::ERROR;
    }

    // Checked up front because the HAL reports a full FIFO as a generic HAL_ERROR.
    if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan) == 0) {
        return SendStatus::QUEUE_FULL;
    }

    // DataLength is a raw DLC, but it is equal to the byte count in the range of 0-8 bytes.
    FDCAN_TxHeaderTypeDef header{
        .Identifier = message.id,
        .IdType = message.extended ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID,
        .TxFrameType = FDCAN_DATA_FRAME,
        .DataLength = message.length,
        .ErrorStateIndicator = FDCAN_ESI_ACTIVE,
        .BitRateSwitch = FDCAN_BRS_OFF,
        .FDFormat = FDCAN_CLASSIC_CAN,
        .TxEventFifoControl = FDCAN_NO_TX_EVENTS,
        .MessageMarker = 0,
    };

    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan, &header, message.data) != HAL_OK) {
        return SendStatus::ERROR;
    }
    return SendStatus::QUEUED;
}

PollStatus Stm32Peripheral::Poll(Message& out) {
    // Acquire pairs with the release store in OnRxFifo0Interrupt(), so the message data is visible.
    if (!rx_pending.load(std::memory_order_acquire)) {
        return PollStatus::EMPTY;
    }

    out = rx_message;

    // Release so the interrupt cannot overwrite the slot until the copy is complete.
    rx_pending.store(false, std::memory_order_release);
    return PollStatus::NEW_MESSAGE;
}

PeripheralStatus Stm32Peripheral::GetStatus() const {
    FDCAN_ProtocolStatusTypeDef protocol{};
    HAL_FDCAN_GetProtocolStatus(&hfdcan, &protocol);

    BusState bus_state = BusState::OK;
    if (protocol.BusOff != 0) {
        bus_state = BusState::BUS_OFF;
    } else if (protocol.ErrorPassive != 0) {
        bus_state = BusState::ERROR_PASSIVE;
    } else if (protocol.Warning != 0) {
        bus_state = BusState::ERROR_WARNING;
    }

    // TODO: tx_count, rx_fifo_overflow_count, tx_buffer_overflow_count, bus_off_count
    return {
        .bus_state = bus_state,
        .rx_count = rx_count.load(std::memory_order_relaxed),
        .rx_buffer_overflow_count = rx_buffer_overflow_count.load(std::memory_order_relaxed),
    };
}

bool Stm32Peripheral::Start() {
    // Accept every standard and extended data frame into FIFO0. Reject remote frames.
    if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_ACCEPT_IN_RX_FIFO0,
                                     FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE) != HAL_OK) {
        return false;
    }
    if (HAL_FDCAN_ActivateNotification(&hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
        return false;
    }
    if (HAL_FDCAN_Start(&hfdcan) != HAL_OK) {
        return false;
    }
    return true;
}

void Stm32Peripheral::OnRxFifo0Interrupt() {
    // Poll until empty because the FIFO can receive multiple frames per interrupt.
    while (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan, FDCAN_RX_FIFO0) > 0) {
        FDCAN_RxHeaderTypeDef header{};
        uint8_t data[MAX_FDCAN_DATA_LENGTH];
        if (HAL_FDCAN_GetRxMessage(&hfdcan, FDCAN_RX_FIFO0, &header, data) != HAL_OK) {
            break;
        }

        // Acquire pairs with the release store in Poll(), so the slot is done being read.
        if (rx_pending.load(std::memory_order_acquire)) {
            // Slot still holds an unread message, so we discard this incoming frame.
            // This issue will be mitigated with the addition of a ring buffer in the future.
            rx_buffer_overflow_count.fetch_add(1, std::memory_order_relaxed);
            continue;
        }

        rx_message.id = header.Identifier;
        rx_message.extended = header.IdType == FDCAN_EXTENDED_ID;

        // DataLength is a raw DLC (0 to 15). Classic CAN treats 9 to 15 as 8 bytes.
        if (header.DataLength > MAX_CAN_DATA_LENGTH) {
            rx_message.length = MAX_CAN_DATA_LENGTH;
        } else {
            rx_message.length = static_cast<uint8_t>(header.DataLength);
        }
        std::memcpy(rx_message.data, data, rx_message.length);

        rx_count.fetch_add(1, std::memory_order_relaxed);

        // Release so Poll() sees the complete message once it sees rx_pending set.
        rx_pending.store(true, std::memory_order_release);
    }
}

} // namespace Platform::Can
