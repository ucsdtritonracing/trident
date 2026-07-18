function(create_stm32_platform_lib board_name)
    set(target_name platform_stm32_${board_name})
    if(TARGET ${target_name})
        return()   # already built for this board
    endif()

    set(board_cubemx_dir ${CMAKE_SOURCE_DIR}/embedded/boards/${board_name}/cubemx)
    add_subdirectory(${board_cubemx_dir} ${CMAKE_BINARY_DIR}/cubemx_${board_name})

    set_target_properties(cubemx PROPERTIES EXCLUDE_FROM_ALL TRUE)

    get_target_property(_mx_app_srcs cubemx SOURCES)
    list(FILTER _mx_app_srcs EXCLUDE REGEX "main\\.c$")
    list(FILTER _mx_app_srcs EXCLUDE REGEX "startup_.*\\.s$")

    add_library(${target_name} STATIC
        ${_mx_app_srcs}
        ${CMAKE_SOURCE_DIR}/embedded/platform/stm32/platform_stm32.cpp
        ${CMAKE_SOURCE_DIR}/embedded/platform/stm32/main_stm32.cpp
    )
    target_link_libraries(${target_name} PUBLIC STM32_Drivers stm32cubemx)
    target_include_directories(${target_name} PUBLIC ${CMAKE_SOURCE_DIR}/embedded/platform/include)

    set(STM32_${board_name}_CUBEMX_DIR ${board_cubemx_dir} CACHE INTERNAL "")
endfunction()