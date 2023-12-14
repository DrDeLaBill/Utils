cmake_minimum_required(VERSION 3.26)

project(Utils VERSION 0.0.1)


if(MSVC)
    message(STATUS "Add MSVC warnings")
    add_compile_options(
        /WX /Wall /W4 /we5240 /we4649 /we4330 /we4926
    )
else()
    message(STATUS "Add GCC warnings")
    add_compile_options(
        -Werror
        -pedantic-errors

        -Wall
        -Wextra
        -Wpedantic

        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wctor-dtor-privacy
        -Wduplicated-branches
        -Wduplicated-cond
        -Wextra-semi
        -Wfloat-equal
        -Wlogical-op
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wredundant-decls
        -Wsign-conversion
        -Wsign-promo
    )
endif()


add_subdirectory(utils)
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    option(UTILS_BEDUG "Enable extended warnings output" OFF)
    option(UTILS_TEST "Enable Utils tests." ON)

    if (UTILS_TEST)
        enable_testing()
        add_subdirectory(test)
    endif()
endif()

set_target_properties(
    utilstest PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)