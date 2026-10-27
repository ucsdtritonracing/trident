#include <cstdio>
#include "core/periodic.h"
#include "core/time.h"
#include "targets/compute_module/target.h"

int main() {
    Target::Init();
    printf("Rear app running!\n");
    return 0;
}
