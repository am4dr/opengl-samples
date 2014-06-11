#pragma comment(lib, "opengl32")
#include "../suika/suika.h"
#include <iostream>

GLuint makeBackgroundProgram() {
    std::vector<suika::shader::ShaderSource> shaderSources;
    shaderSources.push_back(suika::shader::readShaderSource(
        GL_VERTEX_SHADER, "envmap_bg.vert", "envmap_bg_vert"));
    shaderSources.push_back(suika::shader::readShaderSource(
        GL_FRAGMENT_SHADER, "envmap_bg.frag", "envmap_bg_frag"));
    return suika::shader::createShaderProgram(shaderSources);
}
GLuint makeEnvironmentProgram() {
    std::vector<suika::shader::ShaderSource> shaderSources;
    shaderSources.push_back(suika::shader::readShaderSource(
        GL_VERTEX_SHADER, "envmap.vert", "envmap_vert"));
    shaderSources.push_back(suika::shader::readShaderSource(
        GL_FRAGMENT_SHADER, "envmap.frag", "envmap_frag"));
    return suika::shader::createShaderProgram(shaderSources);
}
GLuint createBackgroundVAO(GLuint program, GLubyte restartIndex) {
    GLuint vbo[2];
    glGenBuffers(2, vbo);
    GLuint &verticesBuffer = vbo[0];
    GLuint &indicesBuffer = vbo[1];
    GLfloat positions[] = {
         1.0f, 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,
         1.0f, 1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
    };
    GLfloat colors[] = {
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    };
    GLubyte indices[] = {
        1, 0, 3, 2, 5, 4, 7, 6, restartIndex,
        3, 5, 1, 7, 0, 6, 2, 4
    };
    auto positionLocation = glGetUniformLocation(program, "position");
    auto colorLocation = glGetUniformLocation(program, "color");
    glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(positions) + sizeof(colors), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER,
        0, sizeof(positions), positions);
    glBufferSubData(GL_ARRAY_BUFFER,
        sizeof(positions), sizeof(colors), colors);
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0,
        reinterpret_cast<GLvoid *>(sizeof(positions)));
    glEnableVertexAttribArray(colorLocation);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return vao;
}
void createModelData(GLuint &verticesVBO, GLuint &indicesVBO, const GLubyte restartMarker) {
    GLuint vbo[2];
    glGenBuffers(2, vbo);
    verticesVBO = vbo[0];
    indicesVBO = vbo[1];
    GLfloat vertices[] = {
        0.0f, 1.0f, 0.0f,
        0.75f, 0.0f, 0.75f,
        0.75f, 0.0f, -0.75f,
        -0.75f, 0.0f, -0.75f,
        -0.75f, 0.0f, 0.75f,
        0.0f, -1.0f, 0.0f,
    };
    GLubyte indices[] = {
        0, 1, 2, 3, 4, 1, restartMarker, 5, 4, 3, 2, 1, 4
    };
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * 3 * 6, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLbyte) * 13, indices, GL_STATIC_DRAW);
}
int main(int argc, char **argv) {
    GLFWwindow *window =
        suika::glfw::initializeWindowAndContext(
        600, 600, "environmental mapping", nullptr, nullptr, true);
    glfwSetWindowSizeCallback(window,
        [](GLFWwindow *window, int width, int height){
        int viewportSize = std::min(width, height);
        int widthPadding = std::max((width - viewportSize) / 2, 0);
        int heightPadding = std::max((height - viewportSize) / 2, 0);
        glViewport(widthPadding, heightPadding, viewportSize, viewportSize);
    });
    // 環境用のデータを作成
    GLuint backgroundProgram = makeBackgroundProgram();
    const GLubyte restartIndex = 0xff;
    GLuint backgroundVAO = createBackgroundVAO(backgroundProgram, restartIndex);
    // 環境をテクスチャへレンダリング
    // マッピング対象のモデルのデータを作成
    // マッピング対象へはテクスチャを貼りつつ環境もレンダリング
    return 0;
}