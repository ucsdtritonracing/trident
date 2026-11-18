#pragma once

#include <cstdint>

/// Maximum data length for a classic CAN message.
constexpr int MAX_CAN_DATA_LENGTH = 8;

/// Largest standard (11-bit) CAN ID.
constexpr uint32_t MAX_STANDARD_ID = 0x7FF;

/// Largest extended (29-bit) CAN ID.
constexpr uint32_t MAX_EXTENDED_ID = 0x1FFFFFFF;

namespace Platform::Can {

enum class BusState : uint8_t {
    OK,
    ERROR_WARNING,
    ERROR_PASSIVE,
    BUS_OFF,
};

enum class SendStatus : uint8_t {
    QUEUED,
    QUEUE_FULL,
    ERROR,
};

enum class PollStatus : uint8_t {
    NEW_MESSAGE,
    EMPTY,
    ERROR,
};

struct Message {
    /// Must fit in 11 bits for standard frames, or 29 bits for extended frames.
    uint32_t id;
    /// Standard and extended frames with the same numeric ID are different messages.
    bool extended;
    /// The number of data bytes in the message. Not the DLC.
    uint8_t length;
    uint8_t data[MAX_CAN_DATA_LENGTH];
};

/**
 * @brief Status information for a CAN peripheral.
 *
 * @details The counters only update after the peripheral is initialized. Counters only increase,
 * except when they wrap.
 *
 * @note The terminology used here:
 * - FIFO: a hardware FIFO inside the microcontroller. Does not apply to host implementation.
 * - Buffer: a software buffer used to store or enqueue messages.
 */
struct PeripheralStatus {
    /// The current bus state.
    BusState bus_state = BusState::OK;

    /// The number of received messages, excluding overflowed messages.
    uint32_t rx_count = 0;

    /// The number of received messages lost due to a hardware FIFO overflow.
    uint32_t rx_fifo_overflow_count = 0;

    /// The number of received messages lost due to a software buffer overflow.
    uint32_t rx_buffer_overflow_count = 0;

    /// The number of messages that were successfully transmitted.
    uint32_t tx_count = 0;

    /// The number of enqueued messages that were lost due to a software buffer overflow.
    uint32_t tx_buffer_overflow_count = 0;

    /// The number of times the peripheral entered the bus-off state.
    uint32_t bus_off_count = 0;
};

class Peripheral {
public:
    virtual ~Peripheral() = default;

    /**
     * Send a CAN message over the peripheral.
     *
     * @param message The CAN message to send.
     * @return The status of the send operation.
     */
    virtual SendStatus Send(const Message& message) = 0;

    /**
     * Poll the peripheral for incoming CAN messages.
     *
     * @param out The buffer to store the received message.
     * @return The status of the poll operation.
     */
    virtual PollStatus Poll(Message& out) = 0;

    /**
     * Get the current status of the peripheral.
     *
     * @return The peripheral status.
     */
    virtual PeripheralStatus GetStatus() const = 0;
};

} // namespace Platform::Can
