# Hardware-free libraries shared by every layer, e.g. #include "core/periodic.h"
add_library(core INTERFACE)
target_link_libraries(core INTERFACE embedded_includes)

add_unit_test(core_tests
    SOURCES periodic_test.cpp
    LIBS core)
