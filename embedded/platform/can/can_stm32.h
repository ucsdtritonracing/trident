#pragma once

#include "can.h"

namespace Platform::Can {

class Stm32Peripheral : public Peripheral {
public:
    SendStatus Send(const Message& message) override;
    PollStatus Poll(Message& out) override;
    BusState GetStatus() override;
};

} // namespace Platform::Can
