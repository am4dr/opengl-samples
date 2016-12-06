cmake_minimum_required(VERSION 3.2)
project(suika)

if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options("-std=c++11")
endif()
add_library(suika suika.cpp suika.h)
target_link_libraries(suika
    PRIVATE
        glfw
)
target_include_directories(suika
    PRIVATE
        ${GLFW_INCLUDE_DIR}
)
set(SUIKA_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

