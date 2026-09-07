# Hijack the global project name temporarily
set(ORIGINAL_PROJECT_NAME ${CMAKE_PROJECT_NAME})
set(CMAKE_PROJECT_NAME board_hal)

# Create the target that CubeMX will automatically populate
add_library(${CMAKE_PROJECT_NAME} STATIC)

# Include CubeMX's CMake. It will attach all sources and include paths to our 'board_hal' target.
add_subdirectory(cubemx/cmake/stm32cubemx)

# Restore the original project name
set(CMAKE_PROJECT_NAME ${ORIGINAL_PROJECT_NAME})

# Export the linker script path for the executable to use later
set(BOARD_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/cubemx/STM32G474xx_FLASH.ld CACHE INTERNAL "Board Linker Script")