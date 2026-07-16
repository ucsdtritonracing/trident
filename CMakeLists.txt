cmake_minimum_required(VERSION 3.22)

# Only set the toolchain file when the user requests cross-compiling;
# a bare `cmake -B build/host` configure should use the system's default compiler.
if(DEFINED CMAKE_TOOLCHAIN_FILE)
    # respect whatever the user passed via -DCMAKE_TOOLCHAIN_FILE=...
endif()

project(trident C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CROSSCOMPILING)
    set(CUBEMX_DIR ${CMAKE_SOURCE_DIR}/embedded/boards/compute_module/cubemx)
    add_subdirectory(${CUBEMX_DIR} embedded_cubemx)

    # CubeMX's own default executable target ("cubemx", from
    # `set(CMAKE_PROJECT_NAME cubemx)` in its generated CMakeLists.txt) — unused, exclude it
    set_target_properties(cubemx PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

add_subdirectory(embedded/platform)
add_subdirectory(embedded/app/front)
add_subdirectory(embedded/app/rear)