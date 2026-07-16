set(FW ${CMAKE_SOURCE_DIR}/firmware)
set(LINKER_SCRIPT ${FW}/STM32G474xx_FLASH.ld)
set(COMMON_LINK_OPTIONS
    -T${LINKER_SCRIPT}
    -specs=nosys.specs
    -Wl,--gc-sections
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
)

# --- Entry point 1: sensor_test ---
add_executable(sensor_test.elf
    ${FW}/startup_stm32g474xx.s
    src/main_sensor_test.cpp
)
target_link_libraries(sensor_test.elf PRIVATE stm32_bsp)
target_link_options(sensor_test.elf PRIVATE ${COMMON_LINK_OPTIONS})

# --- Entry point 2: bootloader ---
add_executable(bootloader.elf
    ${FW}/startup_stm32g474xx.s
    src/main_bootloader.cpp
)
target_link_libraries(bootloader.elf PRIVATE stm32_bsp)
target_link_options(bootloader.elf PRIVATE ${COMMON_LINK_OPTIONS})

# Post-build: generate .bin for flashing
foreach(tgt sensor_test bootloader)
    add_custom_command(TARGET ${tgt}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${tgt}.elf> ${tgt}.bin
    )
endforeach()