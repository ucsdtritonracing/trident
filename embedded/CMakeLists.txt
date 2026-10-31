# The single include root for all embedded code
# Headers are included by their full path from here, e.g. #include "platform/can/can.h"
add_library(embedded_includes INTERFACE)
target_include_directories(embedded_includes INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Core library
add_subdirectory(core)

# Git hash of the source tree
add_subdirectory(version)

# Load the platform first so the target can link to it
add_subdirectory(platform)

# Choose the target (hardware configuration). The host build simulates the same target.
# Default to compute_module if target not provided via CLI
set(EMBEDDED_TARGET "compute_module" CACHE STRING "Target hardware configuration")
add_subdirectory(targets/${EMBEDDED_TARGET})

# Define a helper function to link the selected target to an app
function(link_embedded_target APP_NAME)
    # Link the target library
    target_link_libraries(${APP_NAME} PRIVATE embedded_target)

    if(EMBEDDED_PLATFORM STREQUAL "stm32")
        # Inject the target's linker script safely
        if(DEFINED EMBEDDED_TARGET_LINKER_SCRIPT)
            target_link_options(${APP_NAME} PRIVATE -T${EMBEDDED_TARGET_LINKER_SCRIPT})
        else()
            message(FATAL_ERROR "EMBEDDED_TARGET_LINKER_SCRIPT is not defined for ${APP_NAME}")
        endif()
    endif()
endfunction()

# Load the app directory
add_subdirectory(app)
