# Platform interface library, just containing headers
add_library(platform_interface INTERFACE)
target_include_directories(platform_interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Make subdirectories visible to CMake before linking
add_subdirectory(can)

# Add headers to the platform interface library
target_link_libraries(platform_interface INTERFACE
    can_interface
)

if(CMAKE_CROSSCOMPILING)
    # Platform STM32 implementation
    # Links CubeMX generated code
    add_library(platform STATIC platform_stm32.cpp)
    target_link_libraries(platform PRIVATE board_hal)
else()
    # Platform host implementation
    add_library(platform STATIC platform_host.cpp)
endif()

target_link_libraries(platform PRIVATE
    # Required implementation libraries
    can
)

# Add as include directories to allow #include "FILE_NAME" without paths
target_include_directories(platform_interface INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/can
)

# Link platform implementation to the platform interface library
target_link_libraries(platform PUBLIC platform_interface)
