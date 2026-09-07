#include "can_host.h"

#include "can.h"

#include <zmq.hpp>


namespace Platform::Can {

SendStatus HostPeripheral::Send(const Message& message) {
    return SendStatus::ERROR;
}

PollStatus HostPeripheral::Poll(Message& out) {
    return PollStatus::ERROR;
}

BusState HostPeripheral::GetStatus() {
    return BusState::BUS_OFF;
}

}
