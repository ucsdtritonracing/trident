#pragma once

#include <cstdint>

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
    uint32_t id;
    uint8_t length;
    uint8_t data[8];
};

class Peripheral {
public:
    ~Peripheral() = default;

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
     * Get the current bus state of the peripheral.
     *
     * @return The bus state.
     */
    virtual BusState GetStatus() = 0;
};

} // namespace Platform::Can
