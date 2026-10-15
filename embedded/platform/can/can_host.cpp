#include "can_host.h"

#include "can.h"

#include <zmq.hpp>

namespace Platform::Can {

SendStatus HostPeripheral::Send(const Message& message) {
    if (message.length > 8) {
        return SendStatus::ERROR;
    }
    try {
        auto result =
            pub_socket.send(zmq::buffer(message.data, message.length), zmq::send_flags::dontwait);
        if (result) {
            return SendStatus::QUEUED;
        }

        return SendStatus::QUEUE_FULL;
    } catch (const zmq::error_t&) {
        return SendStatus::ERROR;
    }
}

PollStatus HostPeripheral::Poll(Message& out) {
    try {
        zmq::message_t received;

        auto result = sub_socket.recv(received, zmq::recv_flags::dontwait);

        if (!result) {
            return PollStatus::EMPTY;
        }

        if (received.size() > 8) {
            return PollStatus::ERROR;
        }

        out.id = 0;
        out.length = static_cast<uint8_t>(received.size());

        for (size_t i = 0; i < received.size(); i++) {
            out.data[i] = static_cast<const uint8_t*>(received.data())[i];
        }

        return PollStatus::NEW_MESSAGE;

    } catch (const zmq::error_t&) {
        return PollStatus::ERROR; // delete
    }
}

BusState HostPeripheral::GetStatus() { return BusState::BUS_OFF; }

} // namespace Platform::Can
