add_library(platform_interface INTERFACE)
target_include_directories(platform_interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_CROSSCOMPILING)
    # STM32 implementation files
    add_library(platform_stm32 STATIC stm32/platform_stm32.cpp)
    
    # Expose the interface publicly, but keep board_hal (STM32CubeMX generated code) private to the platform
    target_link_libraries(platform_stm32
        PUBLIC platform_interface
        PRIVATE board_hal)
    # Load stm32 headers
    target_include_directories(platform_stm32 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stm32)
else()
    # Host implementation files
    add_library(platform_host STATIC host/platform_host.cpp)
    
    target_link_libraries(platform_host PUBLIC platform_interface)
    # Load host headers
    target_include_directories(platform_host PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/host)
endif()
