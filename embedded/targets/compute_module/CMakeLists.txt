if(CMAKE_CROSSCOMPILING)
    # Hijack the global project name temporarily
    set(ORIGINAL_PROJECT_NAME ${CMAKE_PROJECT_NAME})
    set(CMAKE_PROJECT_NAME board_hal)

    # Create the target that CubeMX will automatically populate
    add_library(${CMAKE_PROJECT_NAME} STATIC)

    # Include CubeMX's CMake. It will attach all sources and include paths to the 'board_hal' target.
    add_subdirectory(cubemx/cmake/stm32cubemx)

    # Restore the original project name
    set(CMAKE_PROJECT_NAME ${ORIGINAL_PROJECT_NAME})

    # Export the linker script path for the executable to use later
    set(BOARD_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/cubemx/STM32G474xx_FLASH.ld CACHE INTERNAL "Board Linker Script")
endif()

# The board library: the Target:: peripherals, wired for stm32 or simulated on host
add_library(board STATIC)
target_link_libraries(board PUBLIC platform_interface)

# Choose between stm32 or host implementation
if(CMAKE_CROSSCOMPILING)
    target_sources(board PRIVATE target_stm32.cpp)
    target_link_libraries(board PRIVATE board_hal)
else()
    target_sources(board PRIVATE target_host.cpp)
endif()

target_link_libraries(board PRIVATE
    # Required implementation libraries
    can
)
