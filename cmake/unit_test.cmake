# Defines add_unit_test(), which builds a GoogleTest executable and registers its tests with ctest.
#
# Usage, from the CMakeLists.txt of the code being tested:
#   add_unit_test(<TEST_EXECUTABLE_NAME>
#       SOURCES <SOURCE>.cpp
#       LIBS    <PRIVATE_LIBRARIES>
#   )
#
# Arguments:
#   NAME     Required. Name of the test executable, e.g. core_tests.
#   SOURCES  Required. The *_test.cpp files. GTest::gtest_main provides main(), so don't define one.
#   LIBS     Optional. Libraries under test, linked privately into the test executable.
#
# Does nothing unless TRIDENT_BUILD_TESTS is ON and this is a host build, so it's safe to call from
# code that's also compiled for the STM32.

function(add_unit_test NAME)
    # Unit tests only run on the host. Skip quietly so the STM32 build never sees GoogleTest.
    if(NOT TRIDENT_BUILD_TESTS OR NOT EMBEDDED_PLATFORM STREQUAL "host")
        return()
    endif()

    # Splits the arguments after NAME into ARG_SOURCES and ARG_LIBS
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBS" ${ARGN})

    if(NOT ARG_SOURCES)
        message(FATAL_ERROR "add_unit_test(${NAME}): SOURCES is required")
    endif()
    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "add_unit_test(${NAME}): unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()

    add_executable(${NAME} ${ARG_SOURCES})
    target_link_libraries(${NAME} PRIVATE ${ARG_LIBS} GTest::gtest_main GTest::gmock)

    # Registers each TEST() with ctest separately so failures are reported by name. PRE_TEST lists
    # the tests when ctest runs instead of right after linking, so building outside the pixi
    # environment (where MinGW's runtime DLLs may not be on PATH) still succeeds on Windows.
    gtest_discover_tests(${NAME} DISCOVERY_MODE PRE_TEST)
endfunction()
