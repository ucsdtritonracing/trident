# Git hash of the source tree, e.g. #include "version/git_hash.h"

# Look for Git on this computer. Sets GIT_EXECUTABLE to its path if found.
find_package(Git QUIET)

# Inputs for cmake/git_hash.cmake
set(GIT_HASH_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
set(GIT_HASH_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/git_hash.h.in")
set(GIT_HASH_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/generated/version/git_hash.h")

# Generate the header now
include("${PROJECT_SOURCE_DIR}/cmake/git_hash.cmake")

# Regenerate on every build so the hash stays current after new commits
add_custom_target(git_hash
    COMMAND "${CMAKE_COMMAND}"
        "-DGIT_EXECUTABLE=${GIT_EXECUTABLE}"
        "-DGIT_HASH_SOURCE_DIR=${GIT_HASH_SOURCE_DIR}"
        "-DGIT_HASH_TEMPLATE=${GIT_HASH_TEMPLATE}"
        "-DGIT_HASH_OUTPUT=${GIT_HASH_OUTPUT}"
        -P "${PROJECT_SOURCE_DIR}/cmake/git_hash.cmake"
    BYPRODUCTS "${GIT_HASH_OUTPUT}"
    COMMENT "Updating Git hash"
)

# Library that gives access to the generated header
add_library(version INTERFACE)
target_include_directories(version INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/generated")
add_dependencies(version git_hash)