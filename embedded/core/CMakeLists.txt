# Hardware-free libraries shared by every layer, e.g. #include "core/periodic.h"
add_library(core INTERFACE)
target_link_libraries(core INTERFACE embedded_includes)
