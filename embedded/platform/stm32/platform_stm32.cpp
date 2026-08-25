#include <cstdio>

#include "platform.h"

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

void Platform::Init() { printf("Running on STM32!\n"); }