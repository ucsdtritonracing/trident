#include <cstdio>

#include "targets/compute_module/target.h"
#include "targets/compute_module/stm32/peripherals.h"
#include "platform/can/can_stm32.h"
#include "platform/i2c/i2c_stm32.h"

#include "main.h"
#include "gpio.h"
#include "fdcan.h"

// Store the peripheral instances that don't need CubeMX handles in a private namespace.
// Limits peripheral access to Target::peripherals below.
namespace {

Platform::I2C::Stm32Peripheral stm32_i2c{};

} // namespace

namespace Target {

constinit Peripherals peripherals{
    .can1 = Stm32::can1,
    .can2 = Stm32::can2,
    .i2c = stm32_i2c,
};

// Defined in cubemx/Core/Src/main.c
// NOLINTNEXTLINE(readability-identifier-naming): CubeMX generated signature
extern "C" void SystemClock_Config();

void Init() {
    // CubeMX initialization calls
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_FDCAN2_Init();
    MX_FDCAN3_Init();

    // Peripheral setup
    if (!Stm32::can1.Start()) {
        Error_Handler();
    }
    if (!Stm32::can2.Start()) {
        Error_Handler();
    }

    printf("Running on STM32!\n");
}

} // namespace Target

// CubeMX's syscalls.c and sysmem.c provide the rest of the syscalls newlib needs.
// Overriding these syscalls so printf compiles until stdio is routed somewhere (e.g. USB or UART).
extern "C" {

// NOLINTBEGIN(readability-identifier-naming): newlib syscall names
int _read(int file, char* ptr, int len) { return 0; }
int _write(int file, char* ptr, int len) { return len; }
// NOLINTEND(readability-identifier-naming)
}
