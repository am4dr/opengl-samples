#pragma comment(lib, "opengl32")
#include <iostream>
#include "../suika/suika.h"
using namespace std;

void init(GLuint &vao, GLuint &vbo) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLfloat vertices[] = {
        -0.90f, -0.90f,
         0.85f, -0.90f,
        -0.90f,  0.85f,
         0.90f, -0.85f,
         0.90f,  0.90f,
        -0.85f,  0.90f,
    };
    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)+sizeof(colors), nullptr,
        GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    vector<suika::shader::ShaderSource> shaderSources;
    shaderSources.push_back(
        suika::shader::readShaderSource(GL_VERTEX_SHADER, "triangles.vert"));
    // ソースコードを直接書いた場合のテスト
    shaderSources.push_back(suika::shader::ShaderSource(GL_FRAGMENT_SHADER, string(
        "#version 430 core\n"
        "out vec4 fColor;\n"
        "in vec4 vfColor;\n"
        "void main() {\n"
        "    fColor = vfColor;\n"
        "}\n"
        "\n"), "fragment shader"));

    GLuint program = suika::shader::createShaderProgram(shaderSources);
    glUseProgram(program);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
        reinterpret_cast<GLvoid *>(sizeof(vertices)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}
//------------------------------------------------------
// main
//
int main(int argc, char** argv) {
    glfwSetErrorCallback(&suika::glfw::error_callback);
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
    GLFWwindow *window = glfwCreateWindow(480, 480, "triangle", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewInit();
    glDebugMessageCallback(suika::gl::debug_message_callback, &std::cerr);

    GLuint vao;
    GLuint vbo;

    init(vao, vbo);
    int width, height;
    while (!glfwWindowShouldClose(window)) {
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glFinish();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
