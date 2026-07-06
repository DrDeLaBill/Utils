cmake_minimum_required(VERSION 3.18)

project(c_only_probe LANGUAGES C)

set(UTILS_FORCE_C_ONLY ON CACHE BOOL "" FORCE)

# This target name intentionally matches CMAKE_PROJECT_NAME because Utils
# auto-links utilslib to the parent target in library mode.
add_library(c_only_probe STATIC probe_dummy.c)

add_subdirectory(".." utils_build)

add_executable(c_only_includes_test test_c_includes.c)
target_link_libraries(c_only_includes_test PRIVATE utilslib)

enable_testing()
add_test(NAME c_only_includes_test COMMAND c_only_includes_test)
