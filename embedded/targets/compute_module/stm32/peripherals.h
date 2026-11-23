#pragma once

#include "platform/can/can_stm32.h"

// Internal peripheral declarations. Code outside targets should use Target::peripherals.
namespace Target::Stm32 {

extern constinit Platform::Can::Stm32Peripheral can1;
extern constinit Platform::Can::Stm32Peripheral can2;

} // namespace Target::Stm32
