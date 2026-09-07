#pragma once

#include "can.h"

#include <zmq.hpp>

namespace Platform::Can {

class HostPeripheral : public Peripheral {
public:
    SendStatus Send(const Message& message) override;
    PollStatus Poll(Message& out) override;
    BusState GetStatus() override;

private:
    zmq::context_t zmq_ctx{1};
};

}
