#pragma comment(lib, "opengl32")
#include "../suika/suika.h"

GLuint makeProgram() {
    std::vector<suika::shader::ShaderSource> shaderSources;
    shaderSources.push_back(suika::shader::readShaderSource(
        GL_VERTEX_SHADER, "texture.vert"));
    shaderSources.push_back(suika::shader::readShaderSource(
        GL_FRAGMENT_SHADER, "texture.frag"));
    return suika::shader::createShaderProgram(shaderSources);
}
GLuint createTextureBuffer() {
    return 0;
}
GLuint createTriangleVAO(GLuint program) {
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
    GLuint program = makeProgram();
    // 描画するプリミティブの頂点座標とテクスチャ座標を
    // 持つバッファを作成し、それを描画するための設定をもつVAOを作成
    GLuint triangleVAO = createTriangleVAO(program);
    // テクスチャの作成
    GLuint textureBuffer = createTextureBuffer();
    // テクスチャを使用するための設定

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
