#include <cstdio>

#include "platform.h"
#include "can_host.h"

#include <zmq.hpp>

// Store global instances of the peripherals in a private namespace.
// Limits peripheral access to just the context object.
namespace {

zmq::context_t zmq_ctx{1};
Platform::Can::HostPeripheral can1{zmq_ctx};
Platform::Can::HostPeripheral can2{zmq_ctx};

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