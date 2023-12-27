cmake_minimum_required(VERSION 3.5.0)

project(utilslib VERSION 0.0.1)


file(GLOB ${PROJECT_NAME}_HEADERS "./*/*.h")
file(GLOB ${PROJECT_NAME}_CPP_SOURCES "./*/*.cpp")
file(GLOB ${PROJECT_NAME}_C_SOURCES "./*/*.c")
set(ALL_SRCS "${${PROJECT_NAME}_CPP_SOURCES};${${PROJECT_NAME}_C_SOURCES};${${PROJECT_NAME}_HEADERS}")
add_library(${PROJECT_NAME} STATIC ${ALL_SRCS})

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
SUBDIRLIST(DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    "${DIR_LIST}"
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)