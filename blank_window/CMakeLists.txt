project(blank_window)

FILE(GLOB ${PROJECT_NAME}_SOURCES "*.cpp")
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}
    PRIVATE ${LIBRARIES_TO_LINK}
)
file(GLOB glsl_SOURCES "*.glsl")
file(COPY ${glsl_SOURCES} DESTINATION "./")
