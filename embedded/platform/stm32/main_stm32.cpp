#include <stdio.h>

#include "platform.h"

extern "C" {

int _close(int) { return -1; }
int _fstat(int, struct stat* st) { return 0; }
int _isatty(int) { return 1; }
int _lseek(int, int, int) { return 0; }
int _read(int, char*, int) { return 0; }
int _write(int, char*, int len) {
    // TODO: Redirect to UART, ITM, SEGGER RTT, etc.
    return len;
}
void* _sbrk(ptrdiff_t) { return (void*)-1; }
}

int main() {
    puts("Running on STM32!");
    app_main();
    return 0;
}