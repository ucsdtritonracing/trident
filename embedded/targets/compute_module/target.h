#pragma once

#include "platform/can/can.h"
#include "platform/i2c/i2c.h"

// The peripherals of the Compute Module. The STM32 build wires them to the CubeMX handles
// (target_stm32.cpp), and the host build simulates them (target_host.cpp).
namespace Target {

struct Peripherals {
    Platform::Can::Peripheral& can1;
    Platform::Can::Peripheral& can2;
    Platform::I2C::Peripheral& i2c;
};

extern constinit Peripherals peripherals;

void Init();

} // namespace Target
