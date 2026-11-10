# The interface that contains only abstract types
# Headers are included by their full path from the embedded directory
add_library(can_interface INTERFACE)
target_link_libraries(can_interface INTERFACE embedded_includes)

# The concrete implementation library of the interface, chosen by platform
add_library(can STATIC can_${EMBEDDED_PLATFORM}.cpp)
target_link_libraries(can PUBLIC can_interface)

if(EMBEDDED_PLATFORM STREQUAL "host")
    # Link third party host packages
    target_link_libraries(can PUBLIC
        cppzmq
    )
elseif(EMBEDDED_PLATFORM STREQUAL "stm32")
    # HAL headers and defines specified in targets/*/CMakeLists.txt.
    # PUBLIC because can_stm32.h will expose HAL types such as FDCAN_HandleTypeDef.
    target_link_libraries(can PUBLIC embedded_target_hal)
endif()
