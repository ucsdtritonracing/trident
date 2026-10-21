# Core logic
add_library(front_core STATIC main_front.cpp)
target_link_libraries(front_core PUBLIC board)

# Executable entry point
add_executable(front main_front.cpp)
target_link_libraries(front PRIVATE front_core)

# Use the helper function to link the board (stm32 vs host)
target_link_board(front)