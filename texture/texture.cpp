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
    // �`�悷��v���~�e�B�u�̒��_���W�ƃe�N�X�`�����W��
    // ���o�b�t�@���쐬���A�����`�悷�邽�߂̐ݒ������VAO���쐬
    GLuint triangleVAO = createTriangleVAO(program);
    // �e�N�X�`���̍쐬
    GLuint textureBuffer = createTextureBuffer();
    // �e�N�X�`�����g�p���邽�߂̐ݒ�

    // �`��̂��߂̐ݒ�
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    // �`��
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        //glDrawArrays(GL_TRIANGLES, 0, 5);
        glFinish();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
