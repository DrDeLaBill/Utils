cmake_minimum_required(VERSION 3.5.0)

project(utilstest VERSION 0.0.1)

message(STATUS "Adding Utils tests")

# Add project files
file(GLOB ${PROJECT_NAME}_HEADERS "./*.h")
file(GLOB ${PROJECT_NAME}_CPP_SOURCES "./*.cpp")
file(GLOB ${PROJECT_NAME}_C_SOURCES "./*.c")
set(ALL_SRCS "${${PROJECT_NAME}_CPP_SOURCES};${${PROJECT_NAME}_C_SOURCES};${${PROJECT_NAME}_HEADERS}")

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -DUTILS_TEST")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUTILS_TEST")

set(CMAKE_SYSTEM_NAME Windows)

# Create project
add_executable(${PROJECT_NAME} ${ALL_SRCS})


target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    "."
)

# Link library
target_link_libraries(
    ${PROJECT_NAME}
    utilslib
)

# Set project properties
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)