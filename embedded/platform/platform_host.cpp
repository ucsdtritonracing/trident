#include <cstdio>

#include "platform.h"
#include "can_host.h"

#include <zmq.hpp>
#include <string>

// namespace
namespace {

constexpr int CAN1_SEND_PORT = 8053;
constexpr int CAN1_RECEIVE_PORT = 8057;
constexpr int CAN2_SEND_PORT = 8054;
constexpr int CAN2_RECEIVE_PORT = 8058;

std::string MakeAddress(int port) { return "tcp://localhost:" + std::to_string(port); }
zmq::context_t zmq_ctx{1};

const std::string can1_send = MakeAddress(CAN1_SEND_PORT);
const std::string can1_receive = MakeAddress(CAN1_RECEIVE_PORT);
const std::string can2_send = MakeAddress(CAN2_SEND_PORT);
const std::string can2_receive = MakeAddress(CAN2_RECEIVE_PORT);

Platform::Can::HostPeripheral can1{zmq_ctx, can1_send, can1_receive};

Platform::Can::HostPeripheral can2{zmq_ctx, can2_send, can2_receive};
} // namespace
// namespace

namespace Platform {

Context Init() {
    printf("Running on host!\n");

    return Context{
        .can1 = can1,
        .can2 = can2,
    };
}

} // namespace Platform