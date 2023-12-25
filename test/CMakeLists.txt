cmake_minimum_required(VERSION 3.5.0)

project(utilstest VERSION 0.0.1)


# Add project files
file(GLOB ${PROJECT_NAME}_HEADERS "./*.h")
file(GLOB ${PROJECT_NAME}_SOURCES "./*.cpp")
set(ALL_SRCS "${${PROJECT_NAME}_SOURCES};${${PROJECT_NAME}_HEADERS}")
add_executable(${PROJECT_NAME} ${ALL_SRCS})

# Link library
target_link_libraries(
    ${PROJECT_NAME}
    utilslib
    # lib
)

# Set project properties
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)