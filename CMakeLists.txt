cmake_minimum_required(VERSION 3.26)

project(Utils VERSION 0.0.1)

message(STATUS "Utils C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Utils C++ compiler: ${CMAKE_CXX_COMPILER}")

add_subdirectory(utils)

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    
    message(STATUS "Utils generated as current project")    

    if(MSVC)
        message(STATUS "Add MSVC warnings")
        add_compile_options(
            /Wall /W4 /we5240 /we4649 /we4330 /we4926
        )
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        message(STATUS "Add GCC warnings")
        add_compile_options(-Wall -Wextra -pedantic -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wconversion)
    else()
        message(ERROR "Unknown compiler")
    endif()

    option(UTILS_BEDUG "Enable extended warnings output" OFF)

    message(STATUS "Enable Utils testing")
    enable_testing()
    add_subdirectory(test)
else()

    message(STATUS "Utils added as a library")

endif()