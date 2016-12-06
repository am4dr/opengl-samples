cmake_minimum_required(VERSION 3.2)
project(study-opengl)

# libraries
add_subdirectory(libs/gl3w)

option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_INSTALL OFF)
add_subdirectory(libs/glfw)

add_subdirectory(suika)

set(LIBRARIES_TO_LINK
    suika
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

install(FILES $<TARGET_FILE:glfw> DESTINATION bin)
include(InstallRequiredSystemLibraries)

