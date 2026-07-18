# Specify board configuration
# e.g. -DFRONT_BOARD=other_board_directory_name
# Default to compute_module
set(FRONT_BOARD "compute_module" CACHE STRING "Board/pinout used for the front app")

add_platform_executable(front
    BOARD ${FRONT_BOARD}
    SOURCES app_front.cpp
)