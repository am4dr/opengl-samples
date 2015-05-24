cmake_minimum_required(VERSION 3.2)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
project(study-opengl)

# libraries
set(LIBRARY_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/libuild)
add_definitions(-DGLEW_STATIC)
add_subdirectory(libs/glew-cmake ${LIBRARY_BUILD_DIR}/glew-cmake)
set(GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew-cmake/include)

option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
add_subdirectory(libs/glfw ${LIBRARY_BUILD_DIR}/glfw)
set(GLFW_INCLUDE_DIR ${GLFW_SOURCE_DIR}/include)

add_subdirectory(suika ${LIBRARY_BUILD_DIR}/suika)

set(LIBRARIES_TO_LINK
    libglew_static
    #libglew_shared
    glfw
    ${GLFW_LIBRARIES}
    suika
)

# includes
include_directories(
    ${GLEW_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${SUIKA_INCLUDE_DIR}
)

# executable
if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options("-std=c++11")
endif()
add_subdirectory(blank_window)
add_subdirectory(triangle)
add_subdirectory(texture)
add_subdirectory(instancing)
add_subdirectory(transform_feedback)


