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
    virtual SendStatus Send(const Message& message) = 0;
    virtual PollStatus Poll(Message& out) = 0;
    virtual BusState GetStatus() = 0;
};

} // namespace Platform::Can
