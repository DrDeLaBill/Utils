cmake_minimum_required(VERSION 3.26)

project(Utils-lib VERSION 0.0.1)


message(STATUS "Utils library found.")


file(GLOB ${PROJECT_NAME}_HEADERS "./*/*.h")
file(GLOB ${PROJECT_NAME}_SOURCES "./*/*.cpp")
set(ALL_SRCS "${${PROJECT_NAME}_SOURCES};${${PROJECT_NAME}_HEADERS}")

add_library(utilslib STATIC ${ALL_SRCS})
target_include_directories(
    utilslib
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

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
    utilslib
    PRIVATE
    "${DIR_LIST}"
)

set_target_properties(
    utilslib PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)