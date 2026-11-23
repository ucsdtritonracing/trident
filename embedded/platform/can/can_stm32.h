#pragma once

#include "platform/can/can.h"

#include <stm32g4xx_hal.h>

#include <atomic>

namespace Platform::Can {

/**
 * @brief CAN peripheral backed by an STM32 FDCAN instance running in classic CAN mode.
 */
class Stm32Peripheral : public Peripheral {
public:
    constexpr explicit Stm32Peripheral(FDCAN_HandleTypeDef& hfdcan) : hfdcan(hfdcan) {}

    /**
     * @copydoc Peripheral::Send
     *
     * @note Not safe to call concurrently on the same peripheral, or from interrupt context.
     */
    SendStatus Send(const Message& message) override;
    PollStatus Poll(Message& out) override;
    PeripheralStatus GetStatus() const override;

    /**
     * @brief Configure and start the FDCAN peripheral.
     *
     * @return True if every HAL call succeeded.
     */
    bool Start();

    /**
     * @brief Drain RX FIFO0 into the receive slot. Called from the HAL RX FIFO0 callback, in
     * interrupt context.
     */
    void OnRxFifo0Interrupt();

private:
    FDCAN_HandleTypeDef& hfdcan;

    static_assert(std::atomic<bool>::is_always_lock_free);
    static_assert(std::atomic<uint32_t>::is_always_lock_free);

    // TODO: Implement ring buffer.
    // Split into separate ticket so they each get their own reviews.
    Message rx_message{};
    std::atomic<bool> rx_pending{false};

    std::atomic<uint32_t> rx_count{0};
    std::atomic<uint32_t> rx_buffer_overflow_count{0};
};

} // namespace Platform::Can
