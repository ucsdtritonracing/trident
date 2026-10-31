#include <cstdio>

#include "targets/compute_module/target.h"
#include "platform/can/can_host.h"

#include <zmq.hpp>

// Store the peripheral instances in a private namespace.
// Limits peripheral access to Target::peripherals below.
namespace {

zmq::context_t zmq_ctx{1};
Platform::Can::HostPeripheral host_can1{zmq_ctx};
Platform::Can::HostPeripheral host_can2{zmq_ctx};

} // namespace

namespace Target {

constinit Peripherals peripherals{
    .can1 = host_can1,
    .can2 = host_can2,
};

void Init() { printf("Running on host!\n"); }

} // namespace Target
