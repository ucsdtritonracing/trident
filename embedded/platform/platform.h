#pragma once

#include "can.h"

namespace Platform {

struct Context {
    Can::Peripheral& can1;
    Can::Peripheral& can2;
};

Context Init();

}