set(CUBEMX_DIR ${CMAKE_SOURCE_DIR}/embedded/boards/compute_module/cubemx)

# Read app-support sources straight from the generated "cubemx" target so this
# tracks whatever CubeMX generates (new peripherals, etc.) with zero manual upkeep.
get_target_property(_mx_app_srcs cubemx SOURCES)
list(FILTER _mx_app_srcs EXCLUDE REGEX "main\\.c$")
list(FILTER _mx_app_srcs EXCLUDE REGEX "startup_.*\\.s$")

# STATIC (not OBJECT) — object libraries don't reliably forward objects through
# more than one hop of target_link_libraries; static libs resolve correctly regardless of depth.
add_library(platform_stm32 STATIC
    ${_mx_app_srcs}
    platform_stm32.cpp
    main_stm32.cpp
)
target_link_libraries(platform_stm32 PUBLIC STM32_Drivers stm32cubemx)
target_include_directories(platform_stm32 PUBLIC ${CMAKE_SOURCE_DIR}/embedded/platform/include)