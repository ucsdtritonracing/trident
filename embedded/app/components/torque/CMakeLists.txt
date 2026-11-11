add_library(torque STATIC pedal_map.cpp)
target_link_libraries(torque PUBLIC embedded_includes)

add_unit_test(pedal_map_tests
    SOURCES pedal_map_test.cpp 
    LIBS torque)