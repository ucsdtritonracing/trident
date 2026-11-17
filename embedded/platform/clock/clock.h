#pragma once
#include "core/time.h"

namespace Platform::Clock {

class Peripheral {
public:
    virtual ~Peripheral() = default;

    virtual Core::Timestamp Now() = 0;
};

} // namespace Platform::Clock
