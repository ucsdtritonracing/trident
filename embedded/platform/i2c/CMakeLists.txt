# The interface that contains only abstract types
# Headers are included by their full path from the embedded directory
add_library(i2c_interface INTERFACE)
target_link_libraries(i2c_interface INTERFACE embedded_includes)

# The concrete implementation library of the interface, chosen by platform
add_library(i2c STATIC i2c_${EMBEDDED_PLATFORM}.cpp)
target_link_libraries(i2c PUBLIC i2c_interface)

if(EMBEDDED_PLATFORM STREQUAL "stm32")
    # Link the STM32 HAL provided by the selected target
    target_link_libraries(i2c PUBLIC
        embedded_target_hal
    )
endif()
