#pragma comment(lib, "opengl32")
#include "../suika/suika.h"
#include <cmath>
#include <iostream>

int main(int argc, char **argv) {
    GLFWwindow *window =
        suika::glfw::initializeWindowAndContext(
        480, 480, "transform feedback", nullptr, nullptr, true);
    glfwSetWindowSizeCallback(window, 
        [](GLFWwindow *window, int width, int height){
        static const int initialValue = 480;
        int viewportSize = std::min(width, height);
        int widthPadding = std::max((width - viewportSize) / 2, 0);
        int heightPadding = std::max((height - viewportSize) / 2, 0);
        glViewport(widthPadding, heightPadding, viewportSize, viewportSize);
    });

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLfloat vertices[] = {
        0.9f, 0.9f, -0.9f, 0.9f, -0.9f, -0.9f, 0.9f, -0.9f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    std::vector<suika::shader::ShaderSource> sources;
    sources.push_back(suika::shader::ShaderSource(GL_VERTEX_SHADER, std::string(
        "#version 330 core\n"
        "layout(location=0) in vec2 position;\n"
        "\n"
        "void main(void) {\n"
        "   gl_Position = vec4(position, 0.0f, 1.0f);\n"
        "}\n"), "vertex shader"));
    sources.push_back(suika::shader::ShaderSource(GL_FRAGMENT_SHADER, std::string(
        "#version 330 core\n"
        "layout(location=0) out vec4 color;"
        "void main(void) {"
        "   color = vec4(0.0f, 0.0f, 0.0f, 1.0f);"
        "}"), "fragment shader"));
    GLuint program = suika::shader::createShaderProgram(sources);
    glUseProgram(program);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glClearColor(0.15f,0.15f,0.15f,1.0f);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawArrays(GL_QUADS, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
