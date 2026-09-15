# Choose the board if we are cross compiling to hardware (stm32)
if(CMAKE_CROSSCOMPILING)
    # Default to compute_module if board not provided via CLI
    set(BOARD "compute_module" CACHE STRING "Target board name")

    # Load the board HAL first so downstream targets can link to it
    add_subdirectory(boards/${BOARD})
endif()

# Load the platform
add_subdirectory(platform)

# Define a helper function to select either stm32 or host
function(target_link_platform TARGET_NAME)
    # Link the platform library
    target_link_libraries(${TARGET_NAME} PRIVATE platform)

    if(CMAKE_CROSSCOMPILING)
        # Inject the board's linker script safely
        if(DEFINED BOARD_LINKER_SCRIPT)
            target_link_options(${TARGET_NAME} PRIVATE -T${BOARD_LINKER_SCRIPT})
        else()
            message(FATAL_ERROR "BOARD_LINKER_SCRIPT is not defined for ${TARGET_NAME}")
        endif()
    endif()
endfunction()

# Load the app directory
add_subdirectory(app)