#include <cstdio>

#include "platform.h"
#include "can_stm32.h"

// Store global instances of the peripherals in a private namespace
// Limits peripheral access to just the context object
namespace {

Platform::Can::Stm32Peripheral can1{};
Platform::Can::Stm32Peripheral can2{};

} // namespace

namespace Platform {

Context Init() {
    printf("Running on STM32!\n");
    return Context{
        .can1 = can1,
        .can2 = can2,
    };
}
} // namespace Platform
extern "C" {

int _close(int file) { return -1; }
int _fstat(int file, struct stat* st) { return 0; }
int _isatty(int file) { return 1; }
int _lseek(int file, int offset, int whence) { return 0; }
int _read(int file, char* ptr, int len) { return 0; }
int _write(int file, char* ptr, int len) { return len; }
void* _sbrk(ptrdiff_t incr) { return (void*)-1; }
}