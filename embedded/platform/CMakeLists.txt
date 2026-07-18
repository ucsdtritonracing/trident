if(NOT CMAKE_CROSSCOMPILING)
    add_subdirectory(host)
endif()
if(CMAKE_CROSSCOMPILING)
    add_subdirectory(stm32)
endif()

# add_platform_executable(name BOARD board_name SOURCES src1 [src2 ...])
# BOARD is only meaningful (and required) when cross-compiling.
function(add_platform_executable name)
    cmake_parse_arguments(ARG "" "BOARD" "SOURCES" ${ARGN})

    add_executable(${name} ${ARG_SOURCES})

    if(CMAKE_CROSSCOMPILING)
        if(NOT ARG_BOARD)
            message(FATAL_ERROR "add_platform_executable(${name}): BOARD is required when cross-compiling")
        endif()

        create_stm32_platform_lib(${ARG_BOARD})
        target_link_libraries(${name} PRIVATE platform_stm32_${ARG_BOARD})

        set(board_cubemx_dir ${CMAKE_SOURCE_DIR}/embedded/boards/${ARG_BOARD}/cubemx)
        target_sources(${name} PRIVATE ${board_cubemx_dir}/startup_stm32g474xx.s)
        target_link_options(${name} PRIVATE
            -T${board_cubemx_dir}/STM32G474xx_FLASH.ld
            -Wl,-Map=${name}.map
        )
        add_custom_command(TARGET ${name} POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${name}> ${name}.bin
            COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${name}>
        )
    else()
        target_link_libraries(${name} PRIVATE platform_host)
    endif()
endfunction()