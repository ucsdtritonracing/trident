#include <cstdio>

#include "targets/compute_module/target.h"
#include "platform/can/can_stm32.h"

// Store the peripheral instances in a private namespace.
// Limits peripheral access to Target::peripherals below.
namespace {

Platform::Can::Stm32Peripheral stm32_can1{};
Platform::Can::Stm32Peripheral stm32_can2{};

} // namespace

namespace Target {

constinit Peripherals peripherals{
    .can1 = stm32_can1,
    .can2 = stm32_can2,
};

void Init() { printf("Running on STM32!\n"); }

} // namespace Target

// Implement syscalls required by C library but not provided by STM32 HAL. These are just stubs for
// now, and should be implemented properly if needed.
extern "C" {

int _close(int file) { return -1; }
int _fstat(int file, struct stat* st) { return 0; }
int _isatty(int file) { return 1; }
int _lseek(int file, int offset, int whence) { return 0; }
int _read(int file, char* ptr, int len) { return 0; }
int _write(int file, char* ptr, int len) {
    // TODO: Redirect to UART, ITM, SEGGER RTT, etc.
    return len;
}
void* _sbrk(ptrdiff_t incr) { return (void*)-1; }
}
