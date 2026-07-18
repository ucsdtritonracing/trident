add_library(platform_host STATIC
    platform_host.cpp
    main_host.cpp
)
target_include_directories(platform_host PUBLIC ${CMAKE_SOURCE_DIR}/embedded/platform/include)