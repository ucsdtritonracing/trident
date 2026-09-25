# Core logic
add_library(rear_core STATIC main_rear.cpp)
target_link_libraries(rear_core PUBLIC platform_interface)

# Executable entry point
add_executable(rear main_rear.cpp)
target_link_libraries(rear PRIVATE rear_core)

# Use the helper function to link the selected platform (stm32 vs host)
target_link_platform(rear)