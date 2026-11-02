
#include <cstdio>
#include <string>

#include "targets/compute_module/target.h"
#include "platform/can/can_host.h"

#include <zmq.hpp>

// Store the peripheral instances in a private namespace.
// Limits peripheral access to Target::peripherals below.
namespace {

struct CanPorts {
    int send;
    int receive;
};

constexpr CanPorts CAN1_PORTS{.send = 8053, .receive = 8057};
constexpr CanPorts CAN2_PORTS{.send = 8054, .receive = 8058};

std::string MakeAddress(int port) { return "tcp://localhost:" + std::to_string(port); }

zmq::context_t zmq_ctx{1};

Platform::Can::HostPeripheral host_can1{zmq_ctx, MakeAddress(CAN1_PORTS.send),
                                        MakeAddress(CAN1_PORTS.receive)};

Platform::Can::HostPeripheral host_can2{zmq_ctx, MakeAddress(CAN2_PORTS.send),
                                        MakeAddress(CAN2_PORTS.receive)};

} // namespace

namespace Target {

constinit Peripherals peripherals{
    .can1 = host_can1,
    .can2 = host_can2,
};

void Init() { printf("Running on host!\n"); }

} // namespace Target
