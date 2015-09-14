#include <suika.h>
using namespace std;

GLuint initVAO() {
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
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)+sizeof(colors), nullptr,
        GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
        reinterpret_cast<GLvoid *>(sizeof(vertices)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    return vao;
}
GLuint createProgram() {
    vector<suika::shader::ShaderSource> shaderSources;
    shaderSources.push_back(
        suika::shader::readShaderSource(GL_VERTEX_SHADER, "triangles_vert.glsl"));
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
    return program;
}
//------------------------------------------------------
// main
//
int main(int argc, char** argv) {
    GLFWwindow *window = 
        suika::glfw::initializeWindowAndContext(
            480, 480, "triangle", nullptr, nullptr, true);
    glfwSetWindowSizeCallback(window, suika::glfw::centeredMaximizedSquareViewport);

    GLuint vao = initVAO();
    glBindVertexArray(vao);
    GLuint program = createProgram();
    glUseProgram(program);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glFinish();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
