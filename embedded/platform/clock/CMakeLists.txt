# The interface that contains only abstract clock types
add_library(clock_interface INTERFACE)
target_link_libraries(clock_interface INTERFACE core)
