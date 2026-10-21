#pragma once

#include "platform/can/can.h"

namespace Platform {

struct Context {
    Can::Peripheral& can1;
    Can::Peripheral& can2;
};

Context Init();

} // namespace Platform