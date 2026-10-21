# The single include root for all embedded code
# Headers are included by their full path from here, e.g. #include "platform/can/can.h"
add_library(trident_includes INTERFACE)
target_include_directories(trident_includes INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Load the platform first so the target can link to it
add_subdirectory(platform)

# Choose the board. The host build simulates the same board.
# Default to compute_module if board not provided via CLI
set(BOARD "compute_module" CACHE STRING "Target board name")
add_subdirectory(targets/${BOARD})

# Define a helper function to link the board (stm32 vs host is chosen by the board library)
function(target_link_board TARGET_NAME)
    # Link the board library
    target_link_libraries(${TARGET_NAME} PRIVATE board)

    if(EMBEDDED_PLATFORM STREQUAL "stm32")
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
