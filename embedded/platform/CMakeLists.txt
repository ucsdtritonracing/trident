# Platform interface library, just containing headers
add_library(platform_interface INTERFACE)

# Make subdirectories visible to CMake before linking
add_subdirectory(can)
add_subdirectory(clock)
add_subdirectory(i2c)

# Add headers to the platform interface library
target_link_libraries(platform_interface INTERFACE
    can_interface
    clock_interface
    i2c_interface
)
