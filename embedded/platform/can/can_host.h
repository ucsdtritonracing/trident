#pragma once

#include "can.h"

#include <zmq.hpp>

namespace Platform::Can {

class HostPeripheral : public Peripheral {
public:
    HostPeripheral(zmq::context_t& zmq_ctx) : zmq_ctx(zmq_ctx) {}
    SendStatus Send(const Message& message) override;
    PollStatus Poll(Message& out) override;
    BusState GetStatus() override;

private:
    zmq::context_t& zmq_ctx;
};

} // namespace Platform::Can
