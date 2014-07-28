project(study-opengl)

cmake_minimum_required(VERSION 2.8)

# GLFW
add_subdirectory(libs/glfw)
include_directories(libs/glfw/include)
# target_link_libraries(myapp glfw ${GLFW_LIBRARIES})

set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
add_subdirectory(blank_window)

