cmake_minimum_required(VERSION 3.22)

project(trident C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(embedded/platform)
add_subdirectory(embedded/app/front)
add_subdirectory(embedded/app/rear)
