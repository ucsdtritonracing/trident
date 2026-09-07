#include <cstdio>

#include "platform.h"
#include "can_host.h"

// Store global instances of the peripherals in a private namespace to limit access to just the context object.
namespace {
    Platform::Can::HostPeripheral can1;
    Platform::Can::HostPeripheral can2;
}

namespace Platform {

Context Init() {
    printf("Running on host!\n");

    can1 = Can::HostPeripheral{};
    can2 = Can::HostPeripheral{};

    return Context{
        .can1 = can1,
        .can2 = can2,
    };
}

}