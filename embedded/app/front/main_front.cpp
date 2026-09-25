#include <cstdio>
#include "platform.h"

int main() {
    Platform::Context platform = Platform::Init();
    printf("Front app running!\n");
    return 0;
}