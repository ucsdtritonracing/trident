#include <cstdio>
#include "platform.h"

int main() {
    Platform::Context platform = Platform::Init();
    printf("Rear app running!\n");
    return 0;
}
