project(blank_window)

#cmake_minimum_required(VERSION 2.8)

#include_directories("${PROJECT_SOURCE_DIR}")

add_executable(blank_window blank_window.cpp)
target_link_libraries(blank_window glfw ${GLFW_LIBRARIES})
