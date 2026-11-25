#include "targets/compute_module/stm32/peripherals.h"

namespace Target::Stm32 {

// No CubeMX I2C handle yet, so the stub is constructed without one.
constinit Platform::I2c::Stm32Peripheral i2c{};

} // namespace Target::Stm32
