cmake_minimum_required(VERSION 3.22)

project(trident C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find third party host packages
if(NOT CMAKE_CROSSCOMPILING)
    find_package(cppzmq REQUIRED)
endif()

# Always build the embedded folder
add_subdirectory(embedded)