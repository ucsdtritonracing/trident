#include <cinttypes>
#include <cstdio>
#include "core/periodic.h"
#include "core/time.h"
#include "targets/compute_module/target.h"
#include "version/git_hash.h"

int main() {
    Target::Init();
    printf("Rear app running!\n");
    printf("Git hash: %08" PRIx32 "\n", Version::GIT_HASH);
    return 0;
}
