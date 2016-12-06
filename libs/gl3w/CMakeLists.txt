cmake_minimum_required(VERSION 3.6)
project(gl3w)
find_package(opengl REQUIRED)
add_library(gl3w gl3w/src/gl3w.c)
target_include_directories(gl3w
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gl3w/include)
target_link_libraries(gl3w ${OPENGL_gl_LIBRARY})
