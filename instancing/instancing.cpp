#pragma comment(lib, "opengl32")
#include "../suika/suika.h"
#include <iostream>

GLuint createProgram() {
    std::vector<suika::shader::ShaderSource> shaderSources;
    shaderSources.push_back(suika::shader::readShaderSource(
        GL_VERTEX_SHADER, "instancing.vert", "instancing_vert"));
    shaderSources.push_back(suika::shader::readShaderSource(
        GL_FRAGMENT_SHADER, "instancing.frag", "instancing_frag"));
    return suika::shader::createShaderProgram(shaderSources);
}
void createModelData(GLuint &verticesVBO, GLuint &indicesVBO, const GLubyte restartMarker) {
    GLuint vbo[2];
    glGenBuffers(2, vbo);
    verticesVBO = vbo[0];
    indicesVBO = vbo[1];
    GLfloat vertices[] = {
         0.0f,  1.0f,  0.0f,
         0.75f, 0.0f,  0.75f,
         0.75f, 0.0f, -0.75f,
        -0.75f, 0.0f, -0.75f,
        -0.75f, 0.0f,  0.75f,
         0.0f, -1.0f,  0.0f,
    };
    GLubyte indices[] = {
        0, 1, 2, 3, 4, restartMarker, 5, 4, 3, 2, 1
    };
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * 3 * 6, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLbyte) * 11, indices, GL_STATIC_DRAW);
}
std::unique_ptr<GLfloat[]> create3elementsData(const GLfloat start, const GLfloat end, const int size) {
    return nullptr;
}
void createInstanceData(GLuint &positionsVBO, GLuint &colorsVBO, const int size) {
    const int number = size * size * size;
    GLuint vbo[2];
    glGenBuffers(2, vbo);
    positionsVBO = vbo[0];
    colorsVBO = vbo[1];
    std::unique_ptr<GLfloat[]> positions(new GLfloat[number * 3]);
    const GLfloat positionStep = 2.0f / (size + 1);
    std::unique_ptr<GLfloat[]> positionRuler(new GLfloat[size]);
    for (int i = 0; i < size; ++i) {
        positionRuler[i] = -1.0f + positionStep * (i + 1);
    }
    for (int i = 0; i < size; ++i) {
        std::cout << positionRuler[i] << std::endl;
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                positions[(i*size*size + j*size + k) * 3 + 0] = positionRuler[k];
                positions[(i*size*size + j*size + k) * 3 + 1] = positionRuler[j];
                positions[(i*size*size + j*size + k) * 3 + 2] = positionRuler[i];
            }
        }
    }
    for (int i = 0; i < number * 3; i += 3) {
        std::cout
            << positions[i + 0] << ", "
            << positions[i + 1] << ", "
            << positions[i + 2] << ", "
            << std::endl;
    }
    glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * number * 3, positions.get(), GL_STATIC_DRAW);

    std::unique_ptr<GLfloat[]> colors(new GLfloat[number * 3]);
    const GLfloat colorStep = 1.0f / (size + 1);
    std::unique_ptr<GLfloat[]> colorRuler(new GLfloat[size]);
    for (int i = 0; i < size; ++i) {
        colorRuler[i] = colorStep * (i + 1);
    }
    for (int i = 0; i < size; ++i) {
        std::cout << colorRuler[i] << std::endl;
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                colors[(i*size*size + j*size + k) * 3 + 0] = colorRuler[k];
                colors[(i*size*size + j*size + k) * 3 + 1] = colorRuler[j];
                colors[(i*size*size + j*size + k) * 3 + 2] = colorRuler[i];
            }
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * number * 3, colors.get(), GL_STATIC_DRAW);
}
int main(int argc, char **argv) {
    GLFWwindow *window =
        suika::glfw::initializeWindowAndContext(
        600, 600, "instancing", nullptr, nullptr, true);
    glfwSetWindowSizeCallback(window,
        [](GLFWwindow *window, int width, int height){
        int viewportSize = std::min(width, height);
        int widthPadding = std::max((width - viewportSize) / 2, 0);
        int heightPadding = std::max((height - viewportSize) / 2, 0);
        glViewport(widthPadding, heightPadding, viewportSize, viewportSize);
    });
    GLuint shaderProgram = createProgram();
    // インスタンス化するモデルのデータをバッファに入れる
    GLuint modelVerticesBuffer;
    GLuint modelElementsIndicesBuffer;
    createModelData(modelVerticesBuffer, modelElementsIndicesBuffer, 0xff);
    // インスタンスごとに設定するデータをバッファに入れる
    GLuint instancePositionBuffer;
    GLuint instanceColorBuffer;
    const int instancingSize = 2;
    const int instanceNumber = instancingSize * instancingSize * instancingSize;
    createInstanceData(
        instancePositionBuffer, instanceColorBuffer, instancingSize);
    // プログラムからattribute変数の位置を取得する
    GLint positionLocation = glGetAttribLocation(shaderProgram, "position");
    GLint instancePositionLocation = glGetAttribLocation(shaderProgram, "instancePosition");
    GLint instanceColorLocation = glGetAttribLocation(shaderProgram, "instanceColor");
    // 描画に必要なデータをVAOに設定する
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelElementsIndicesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, modelVerticesBuffer);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(positionLocation);
    // インスタンスごとのAttrib設定
    glBindBuffer(GL_ARRAY_BUFFER, instancePositionBuffer);
    glVertexAttribDivisor(instancePositionLocation, 1);
    glVertexAttribPointer(instancePositionLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(instancePositionLocation);
    glBindBuffer(GL_ARRAY_BUFFER, instanceColorBuffer);
    glVertexAttribDivisor(instanceColorLocation, 1);
    glVertexAttribPointer(instanceColorLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(instanceColorLocation);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    // 描画処理
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(0xff);
    glBindVertexArray(vao);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElementsInstanced(GL_TRIANGLE_FAN, 11, GL_UNSIGNED_BYTE, 0, instanceNumber);
        glFinish();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}