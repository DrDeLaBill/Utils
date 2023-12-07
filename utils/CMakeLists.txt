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
set_target_properties(
    utilslib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)