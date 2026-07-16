if(CMAKE_CROSSCOMPILING)
    add_subdirectory(stm32)
    set(PLATFORM_IMPL_TARGET platform_stm32 CACHE INTERNAL "")
else()
    add_subdirectory(host)
    set(PLATFORM_IMPL_TARGET platform_host CACHE INTERNAL "")
endif()

# add_platform_executable(name source1 [source2 ...])
# Hides which main_*.cpp, which startup file, which linker script, which HAL/libc
# gets used — callers (app/*) never need to know or care.
function(add_platform_executable name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} PRIVATE ${PLATFORM_IMPL_TARGET})

    if(CMAKE_CROSSCOMPILING)
        set(CUBEMX_DIR ${CMAKE_SOURCE_DIR}/embedded/boards/compute_module/cubemx)
        target_sources(${name} PRIVATE ${CUBEMX_DIR}/startup_stm32g474xx.s)
        target_link_options(${name} PRIVATE
            -T${CUBEMX_DIR}/STM32G474xx_FLASH.ld
            -Wl,-Map=${name}.map
        )
        add_custom_command(TARGET ${name} POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${name}> ${name}.bin
            COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${name}>
        )
    endif()
endfunction()