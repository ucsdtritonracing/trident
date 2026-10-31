# Core logic
add_library(rear_core STATIC main_rear.cpp)
target_link_libraries(rear_core PUBLIC embedded_target version)

# Executable entry point
add_executable(rear main_rear.cpp)
target_link_libraries(rear PRIVATE rear_core)

# Use the helper function to link the selected target
link_embedded_target(rear)