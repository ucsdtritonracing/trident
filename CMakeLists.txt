cmake_minimum_required(VERSION 3.22)

project(trident C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# The platform the embedded code is built for. Set by the CMake presets.
# Each value matches the suffix of its implementation files, e.g. can_host.cpp / can_stm32.cpp
set(EMBEDDED_PLATFORM "host" CACHE STRING "Platform the embedded code is built for")
set(EMBEDDED_PLATFORMS host stm32)
set_property(CACHE EMBEDDED_PLATFORM PROPERTY STRINGS ${EMBEDDED_PLATFORMS})
if(NOT EMBEDDED_PLATFORM IN_LIST EMBEDDED_PLATFORMS)
    message(FATAL_ERROR "Unknown EMBEDDED_PLATFORM '${EMBEDDED_PLATFORM}' (expected one of: ${EMBEDDED_PLATFORMS})")
endif()

# Builds unit tests. Defaults to ON.
# Disable this option to prevent building the unit tests. E.g. -DTRIDENT_BUILD_TESTS=OFF
option(TRIDENT_BUILD_TESTS "Build unit tests (host only)" ON)

# Find third party host packages
if(EMBEDDED_PLATFORM STREQUAL "host")
    add_subdirectory(shared/protocol)
    
    find_package(cppzmq REQUIRED)
    if(TRIDENT_BUILD_TESTS)
        find_package(GTest REQUIRED)
        include(GoogleTest)
        enable_testing()
    endif()
endif()

# Force CMake to emit ALL include paths into compile_commands.json
set(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES "")
set(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES "")

# Provides add_unit_test(). It does nothing unless TRIDENT_BUILD_TESTS is ON and this is a host build.
include(cmake/unit_test.cmake)

# Always build the embedded folder
add_subdirectory(embedded)
