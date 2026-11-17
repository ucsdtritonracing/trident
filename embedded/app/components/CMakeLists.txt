add_subdirectory(torque)
add_library(app_components INTERFACE)
target_link_libraries(app_components INTERFACE core torque)

add_unit_test(app_components_tests
    SOURCES scheduler_test.cpp
    LIBS app_components)
