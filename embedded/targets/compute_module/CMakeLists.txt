if(EMBEDDED_PLATFORM STREQUAL "stm32")
    # Hijack the global project name temporarily
    set(ORIGINAL_PROJECT_NAME ${CMAKE_PROJECT_NAME})
    set(CMAKE_PROJECT_NAME embedded_target_cubemx)

    # Create the target that CubeMX will automatically populate
    add_library(${CMAKE_PROJECT_NAME} STATIC)

    # Include CubeMX's CMake. It will attach all sources and include paths to the
    # 'embedded_target_cubemx' target.
    add_subdirectory(cubemx/cmake/stm32cubemx)

    # Restore the original project name
    set(CMAKE_PROJECT_NAME ${ORIGINAL_PROJECT_NAME})

    # Export the linker script path for the executable to use later
    set(EMBEDDED_TARGET_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/cubemx/STM32G474xx_FLASH.ld CACHE INTERNAL "Target Linker Script")
endif()

# The target library: the Target:: peripherals, wired for stm32 or simulated on host
add_library(embedded_target STATIC target_${EMBEDDED_PLATFORM}.cpp)
target_link_libraries(embedded_target PUBLIC platform_interface)

if(EMBEDDED_PLATFORM STREQUAL "stm32")
    target_link_libraries(embedded_target PRIVATE embedded_target_cubemx)
endif()

target_link_libraries(embedded_target PRIVATE
    # Required implementation libraries
    can
)
