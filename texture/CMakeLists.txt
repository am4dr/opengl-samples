project(texture)

file(GLOB ${PROJECT_NAME}_SOURCES "*.cpp")
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}
    PRIVATE ${LIBRARIES_TO_LINK}
)
file(GLOB glsl_SOURCES "*.glsl")
file(COPY ${glsl_SOURCES} DESTINATION "./")

install(FILES $<TARGET_FILE:${PROJECT_NAME}> DESTINATION "bin")
install(FILES ${glsl_SOURCES} DESTINATION "bin")