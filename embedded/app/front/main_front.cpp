#include <cstdio>
#include "targets/compute_module/target.h"

int main() {
    Target::Init();
    printf("Front app running!\n");
    return 0;
}