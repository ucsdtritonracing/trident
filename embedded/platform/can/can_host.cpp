#include "platform/can/can_host.h"

#include "platform/can/can.h"

#include <zmq.hpp>

namespace Platform::Can {

SendStatus HostPeripheral::Send(const Message& message) { return SendStatus::ERROR; }

PollStatus HostPeripheral::Poll(Message& out) { return PollStatus::ERROR; }

BusState HostPeripheral::GetStatus() { return BusState::BUS_OFF; }

} // namespace Platform::Can
