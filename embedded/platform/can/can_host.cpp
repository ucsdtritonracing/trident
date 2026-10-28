#include "can_host.h"

#include "can.h"

#include <zmq.hpp>
#define MAX_CAN_DATA_LENGTH 8

namespace Platform::Can {


SendStatus HostPeripheral::Send(const Message& message) {
    if (message.length > MAX_CAN_DATA_LENGTH) {
        return SendStatus::ERROR;
    }
    zmq::send_result_t result;
    try {
        result = pub_socket.send(zmq::buffer(message.data, message.length),
                                 zmq::send_flags::dontwait);
    } catch (const zmq::error_t&) {
        return SendStatus::ERROR;
    }

    if (result) {
        return SendStatus::QUEUED;
    } 
    return SendStatus::QUEUE_FULL;
}

PollStatus HostPeripheral::Poll(Message& out) {
    zmq::message_t received;
    zmq::recv_result_t result;

    try {
        result = sub_socket.recv(received, zmq::recv_flags::dontwait);
    } catch (const zmq::error_t&) {
        return PollStatus::ERROR;
    }

    if (!result) {
        return PollStatus::EMPTY;
    }

    if (received.size() > MAX_CAN_DATA_LENGTH) {
        return PollStatus::ERROR;
    }
    
    out.id = 0;
    out.length = static_cast<uint8_t>(received.size());
    for (size_t i = 0; i < received.size(); i++) {
        out.data[i] = static_cast<const uint8_t*>(received.data())[i];
    }
    return PollStatus::NEW_MESSAGE;
}

BusState HostPeripheral::GetStatus(){ 
    return BusState::BUS_OFF; 
}

} 
