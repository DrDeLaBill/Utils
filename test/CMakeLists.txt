cmake_minimum_required(VERSION 3.26)

project(StorageAT-test VERSION 0.0.1)


message(STATUS "Utils tests enabled.")


add_library(utillib "../utils/Log/CodeStopWatch.cpp;../utils/Time/GTime.cpp")

file(GLOB ${PROJECT_NAME}_HEADERS "./*.h")
file(GLOB ${PROJECT_NAME}_SOURCES "./*.cpp")
set(ALL_SRCS "${${PROJECT_NAME}_SOURCES};${${PROJECT_NAME}_HEADERS}")
add_executable(utilstest ${ALL_SRCS})

# TODO: cmake finder
SET(DIR_LIST "")
MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${curdir}/${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()
SUBDIRLIST(DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../utils")
target_include_directories(
    utilstest
    PRIVATE
    "${DIR_LIST}"
)

target_link_libraries(
    utilstest
    INTERFACE // TODO: remove
    utilslib
)
set_target_properties(
    utilstest PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)