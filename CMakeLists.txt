cmake_minimum_required(VERSION 3.18)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(utilslib VERSION 0.0.1 LANGUAGES C CXX)
else()
    project(utilslib VERSION 0.0.1 LANGUAGES C)
endif()

option(UTILS_FORCE_C_ONLY "Force C-only mode (ignore .cpp/.hpp even if C++ compiler is available)" OFF)

if(NOT UTILS_FORCE_C_ONLY)
    enable_language(CXX OPTIONAL)
endif()

set(UTILS_HAS_CXX FALSE)
if(CMAKE_CXX_COMPILER_LOADED AND NOT UTILS_FORCE_C_ONLY)
    set(UTILS_HAS_CXX TRUE)
endif()


message(STATUS "Utils C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Utils C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Utils C++ enabled: ${UTILS_HAS_CXX}")
message(STATUS "Utils force C-only: ${UTILS_FORCE_C_ONLY}")


option(DEBUG "Enable DEBUG output" OFF)


set(_file "${CMAKE_CURRENT_LIST_DIR}/search.cmake")
if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "Unable to find macros search.cmake")
endif()
include(${_file})
unset(_file)


# Ищем заголовочные файлы
FILE_PATHS_EXT(h_paths   "${CMAKE_CURRENT_SOURCE_DIR}/inc" "h")
if(UTILS_HAS_CXX)
    FILE_PATHS_EXT(hpp_paths "${CMAKE_CURRENT_SOURCE_DIR}/inc" "hpp")
else()
    set(hpp_paths "")
endif()

# Указываем исходные файлы проекта
FILE_PATHS_EXT(c_paths   "${CMAKE_CURRENT_SOURCE_DIR}/src" "c")
if(UTILS_HAS_CXX)
    FILE_PATHS_EXT(cpp_paths "${CMAKE_CURRENT_SOURCE_DIR}/src" "cpp")
else()
    set(cpp_paths "")
    message(STATUS "C++ compiler is not available: .cpp/.hpp files are ignored")
endif()
FILE_PATHS_EXT(s_paths   "${CMAKE_CURRENT_SOURCE_DIR}/src" "s")
set(${PROJECT_NAME}_SOURCES "${h_paths};${hpp_paths};${c_paths};${cpp_paths};${s_paths}")
add_library(
    ${PROJECT_NAME} 
    STATIC 
    ${${PROJECT_NAME}_SOURCES}
)

# Указываем директории с заголовочными файлами
FILE_DIRECTORIES_EXT(h_dirs   "${CMAKE_CURRENT_SOURCE_DIR}/inc" "h")
FILE_DIRECTORIES_EXT(hpp_dirs "${CMAKE_CURRENT_SOURCE_DIR}/inc" "hpp")
set(${PROJECT_NAME}_INCLUDES "${h_dirs};${hpp_dirs}")
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${${PROJECT_NAME}_INCLUDES}
)


if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})

    if(NOT UTILS_HAS_CXX)
        message(FATAL_ERROR "Standalone build requires a C++ compiler")
    endif()
    
    message(STATUS "Utils generated as current project")

    if(MSVC)
        message(STATUS "Add MSVC warnings")
        add_compile_options(
            /Wall /W4 /we5240 /we4649 /we4330 /we4926 /Zc:__cplusplus /std:c++17
        )
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        message(STATUS "Add GCC warnings")
        add_compile_options(-Wall -Wextra -pedantic -Wmissing-include-dirs -std=gnu++17)
        if (DEBUG)
            message(STATUS "Build gsystem with DEBUG")
            set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
        endif()
    else()
        message(ERROR "Unknown compiler")
    endif()

    option(UTILS_BEDUG "Enable extended warnings output" OFF)

    message(STATUS "Enable Utils testing")
    enable_testing()
    add_subdirectory(test)

    message(STATUS "C compiler   : ${CMAKE_C_COMPILER}")
    message(STATUS "C++ compiler : ${CMAKE_CXX_COMPILER}")
    message(STATUS "Linker       : ${LINKER_SCRIPT_PATH}")
    message(STATUS "Building     : ${PROJECT_NAME}")
    message(STATUS "C FLAGS      : ${CMAKE_C_FLAGS}")
    message(STATUS "C++ FLAGS    : ${CMAKE_CXX_FLAGS}")
    message(STATUS "ASM FLAGS    : ${CMAKE_ASM_FLAGS}")
    message(STATUS "Linker FLAGS : ${CMAKE_LINKER_FLAGS}")
    
    # Set project properties
    set_target_properties(
        ${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        LINKER_LANGUAGE CXX
    )

else()

    message(STATUS "Utils added as a library")

    message(STATUS "${PROJECT_NAME} includes:")
    foreach(dir ${${PROJECT_NAME}_INCLUDES})
        message(STATUS "${dir}")
    endforeach()
    message(STATUS "${PROJECT_NAME} sources:")
    foreach(src ${${PROJECT_NAME}_SOURCES})
        message(STATUS "${src}")
    endforeach()
    
    target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${${PROJECT_NAME}_INCLUDES}
    )
    target_link_libraries(
        ${CMAKE_PROJECT_NAME}
        ${PROJECT_NAME}
    )

endif()