#include <cstdio>
#include "targets/compute_module/target.h"
#include "version/git_hash.h"

int main() {
    Target::Init();
    printf("Front app running!\n");
    printf("Git hash: %s\n", Version::GIT_HASH);
    return 0;
}