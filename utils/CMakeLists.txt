cmake_minimum_required(VERSION 3.26)

project(utils VERSION 0.0.1)

message(STATUS "utils library found")

file(GLOB ALL_HEADERS "./*/*.h")

add_library(utils STATIC ${ALL_HEADERS})


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

SUBDIRLIST(DIR_LIST ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(
    utils
    PRIVATE
    "${DIR_LIST}"
)

message(WARNING "${DIR_LIST}")


set_target_properties(
    utils PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)