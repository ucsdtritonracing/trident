# Specify board configuration
# e.g. -DREAR_BOARD=other_board_directory_name
# Default to compute_module
set(REAR_BOARD "compute_module" CACHE STRING "Board/pinout used for the rear app")

add_platform_executable(rear
    BOARD ${REAR_BOARD}
    SOURCES app_rear.cpp
)