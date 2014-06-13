#pragma comment(lib, "opengl32")
#include "../suika/suika.h"

GLuint createTriangleVAO(const GLuint program, const GLuint triangleNumber) {
    GLuint positionLocation = glGetAttribLocation(program, "position");
    GLuint texCoordLocation = glGetAttribLocation(program, "texCoord");
    // 頂点のattribute用の配列を確保
    std::unique_ptr<GLfloat[]> positions(new GLfloat[triangleNumber * 3 * 2]);
    std::unique_ptr<GLfloat[]> texCoords(new GLfloat[triangleNumber * 3 * 2]);
    // TODO 配列にデータを格納する
    static const GLfloat data[] = { 0.86f, 0.75f, -0.86f, 0.75f, 0.0f, -0.75f };
    for (int i = 0; i < 6; ++i) {
        positions[i] = data[i];
        texCoords[i] = data[i] / 2 + 0.5f;
    }
    // バッファを作って入れる
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    static const int bufferSize = sizeof(GLfloat) * 3 * 2;
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
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    static const GLuint textureWidth = 8;
    static const GLuint textureHeight = 8;
    static const GLfloat pixels[textureWidth * textureHeight] = {
        1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    };
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, textureWidth, textureHeight);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, textureWidth, textureHeight, GL_RED, GL_FLOAT, pixels);
    // グレースケールにしたい。
    // R値のみを読み込んでいるのでここでRをRGB各値として扱うように設定する。
    static const GLint swizzles[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzles);
    // テクスチャが縮小されるときにどのテクセルの値をどう取るか。
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // テクスチャ座標からテクセルの値をどう取るか。
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Mipmapの作成。
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture;
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
    static const GLuint triangleNumber = 1;
    GLuint triangleVAO = createTriangleVAO(program, triangleNumber);
    // テクスチャの作成。今回はテクスチャひとつのみでTEXTURE0を使用。
    GLuint textureBuffer = createTextureBuffer();
    // 描画のための設定
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glUseProgram(program);
    glBindVertexArray(triangleVAO);
    // 描画
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, triangleNumber * 3);
        glFinish();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
