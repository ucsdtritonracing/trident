# The interface that contains only abstract types
# Make headers in this directory available to anything that links the interface
add_library(can_interface INTERFACE)
target_include_directories(can_interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# The concrete implementation library of the interface
add_library(can STATIC)
target_link_libraries(can PUBLIC can_interface)

# Expose this directory publicly
# Allows consumers of the implementation library to include stm32 or host headers
target_include_directories(can PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Choose between stm32 or host implementation
if(CMAKE_CROSSCOMPILING)
    target_sources(can PRIVATE can_stm32.cpp)
else()
    target_sources(can PRIVATE can_host.cpp)

    # Link third party host packages
    target_link_libraries(can PRIVATE
        cppzmq
    )
endif()