#include <cstdio>

#include "platform.h"
#include "can_host.h"

#include <zmq.hpp>

// Store global instances of the peripherals in a private namespace
// Limits peripheral access to just the context object
namespace {

struct CanPorts {
    int send;
    int receive;
};

constexpr CanPorts CAN1_PORTS{.send = 8053, .receive = 8057};
constexpr CanPorts CAN2_PORTS{.send = 8054, .receive = 8058};

std::string MakeAddress(int port) { return "tcp://localhost:" + std::to_string(port); }
zmq::context_t zmq_ctx{1};

Platform::Can::HostPeripheral can1{zmq_ctx, MakeAddress(CAN1_PORTS.send),
                                   MakeAddress(CAN1_PORTS.receive)};

Platform::Can::HostPeripheral can2{zmq_ctx, MakeAddress(CAN2_PORTS.send),
                                   MakeAddress(CAN2_PORTS.receive)};
} // namespace
namespace Platform {

Context Init() {
    printf("Running on host!\n");
    return Context{
        .can1 = can1,
        .can2 = can2,
    };
}
} // namespace Platform