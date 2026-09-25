# Core logic
add_library(front_core STATIC main_front.cpp)
target_link_libraries(front_core PUBLIC platform_interface)

# Executable entry point
add_executable(front main_front.cpp)
target_link_libraries(front PRIVATE front_core)

# Use the helper function to link the selected platform (stm32 vs host)
target_link_platform(front)