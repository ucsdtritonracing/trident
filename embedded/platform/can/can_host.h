#pragma once

#include "can.h"

#include <zmq.hpp>
#include <string>

namespace Platform::Can {

class HostPeripheral : public Peripheral {
public:
    HostPeripheral(zmq::context_t& zmq_ctx, const std::string& pub_endpoint,
                   const std::string& sub_endpoint)
        : zmq_ctx(zmq_ctx), pub_socket(zmq_ctx, zmq::socket_type::pub),
          sub_socket(zmq_ctx, zmq::socket_type::sub) {
        sub_socket.set(zmq::sockopt::subscribe, "");

        if (!pub_endpoint.empty()) {
            pub_socket.bind(pub_endpoint);
        }

        if (!sub_endpoint.empty()) {
            sub_socket.connect(sub_endpoint);
        }
    }
    SendStatus Send(const Message& message) override;
    PollStatus Poll(Message& out) override;
    BusState GetStatus() override;

private:
    zmq::context_t& zmq_ctx;
    zmq::socket_t pub_socket;
    zmq::socket_t sub_socket;
};

} // namespace Platform::Can
