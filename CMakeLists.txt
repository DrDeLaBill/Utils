cmake_minimum_required(VERSION 3.26)

project(glibrary VERSION 0.0.1)

file(GLOB ALL_SOURCES "./*.cpp")


# SET(DIR_LIST "")
# MACRO(SUBDIRLIST result curdir)
#   FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
#   SET(dirlist "")
#   FOREACH(child ${children})
#     IF(IS_DIRECTORY ${curdir}/${child})
#       LIST(APPEND dirlist ${curdir}/${child})
#     ENDIF()
#   ENDFOREACH()
#   SET(${result} ${dirlist})
# ENDMACRO()
# SUBDIRLIST(DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/utils")
# target_include_directories(
#     glibrary
#     PRIVATE
#     "${DIR_LIST}"
# )


add_subdirectory(utils)

add_executable(glibrary ${ALL_SOURCES})

set_target_properties(
    glibrary PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)