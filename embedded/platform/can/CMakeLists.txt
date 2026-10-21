# The interface that contains only abstract types
# Headers are included by their full path from the embedded directory
add_library(can_interface INTERFACE)
target_link_libraries(can_interface INTERFACE trident_includes)

# The concrete implementation library of the interface
add_library(can STATIC)
target_link_libraries(can PUBLIC can_interface)

# Choose between stm32 or host implementation
if(CMAKE_CROSSCOMPILING)
    target_sources(can PRIVATE can_stm32.cpp)
else()
    target_sources(can PRIVATE can_host.cpp)

    # Link third party host packages
    target_link_libraries(can PUBLIC
        cppzmq
    )
endif()
