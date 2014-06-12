#pragma comment(lib, "opengl32")
#include "../suika/suika.h"

GLuint createTriangleVAO(const GLuint program, const GLuint triangleNumber) {
    GLuint positionLocation = glGetAttribLocation(program, "position");
    GLuint texCoordLocation = glGetAttribLocation(program, "texCoord");
    // 頂点のattribute用の配列を確保
    std::unique_ptr<GLfloat[]> positions(new GLfloat[triangleNumber * 3 * 2]);
    std::unique_ptr<GLfloat[]> texCoords(new GLfloat[triangleNumber * 3 * 2]);
    // TODO 配列にデータを格納する

    // バッファを作って入れる
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    const int bufferSize = sizeof(GLfloat) * 3 * 2;
    glBufferData(GL_ARRAY_BUFFER, bufferSize * 2, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, positions.get());
    glBufferSubData(GL_ARRAY_BUFFER, bufferSize, bufferSize, texCoords.get());
    // VAOの設定
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0,
        reinterpret_cast<GLvoid *>(bufferSize));
    glEnableVertexAttribArray(positionLocation);
    glEnableVertexAttribArray(texCoordLocation);
    glBindVertexArray(0);
    return vao;
}
GLuint createTextureBuffer() {
    return 0;
}
int main(int argc, char** argv) {
    GLFWwindow *window =
        suika::glfw::initializeWindowAndContext(
        600, 600, "texture2D", nullptr, nullptr, true);
    glfwSetWindowSizeCallback(window,
        [](GLFWwindow *window, int width, int height){
        int viewportSize = std::min(width, height);
        int widthPadding = std::max((width - viewportSize) / 2, 0);
        int heightPadding = std::max((height - viewportSize) / 2, 0);
        glViewport(widthPadding, heightPadding, viewportSize, viewportSize);
    });
    GLuint program = suika::shader::makeProgram("texture.vert", "texture.frag");
    // 描画するプリミティブの頂点座標とテクスチャ座標を
    // 持つバッファを作成し、それを描画するための設定をもつVAOを作成
    const GLuint triangleNumber = 10;
    GLuint triangleVAO = createTriangleVAO(program, triangleNumber);
    // TODO テクスチャの作成
    GLuint textureBuffer = createTextureBuffer();
    // TODO テクスチャを使用するための設定

    // 描画のための設定
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    // 描画
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        //glDrawArrays(GL_TRIANGLES, 0, 5);
        glFinish();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
