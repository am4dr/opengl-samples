#pragma comment(lib, "opengl32")
#include "../suika/suika.h"

GLuint createProgram() {
    std::vector<suika::shader::ShaderSource> shaderSources;
    shaderSources.push_back(suika::shader::readShaderSource(
        GL_VERTEX_SHADER, "instancing.vert", "instancing_vert"));
    shaderSources.push_back(suika::shader::readShaderSource(
        GL_FRAGMENT_SHADER, "instancing.frag", "instancing_frag"));
    return suika::shader::createShaderProgram(shaderSources);
}
void createModelBufferData(GLuint &verticesVBO, GLuint &indicesVBO) {
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
    GLbyte indices[] = {
        0, 1, 2, 3, 4, -1, 5, 4, 3, 2, 1
    };
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * 3 * 6, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLbyte) * 11, indices, GL_STATIC_DRAW);
}
void createInstanceData(GLuint &positionsVBO, GLuint &colorsVBO, const int number) {

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
    // �C���X�^���X�����郂�f���̃f�[�^���o�b�t�@�ɓ����
    GLuint modelVerticesBuffer;
    GLuint modelElementsIndicesBuffer;
    createModelBufferData(modelVerticesBuffer, modelElementsIndicesBuffer);
    // �C���X�^���X���Ƃɐݒ肷��f�[�^���o�b�t�@�ɓ����
    GLuint instancePositionBuffer;
    GLuint instanceColorBuffer;
    const int instanceNumber = 10 * 10 * 10;
    createInstanceData(
        instancePositionBuffer, instanceColorBuffer, instanceNumber);
    // �`��ɕK�v�ȃf�[�^��VAO�ɐݒ肷��
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, modelVerticesBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelElementsIndicesBuffer);
    // �C���X�^���X���Ƃ�Attrib�ݒ�
    // glVertexAttribDivisor(index, divisor)
    // glVertexAttribPointer(index, ...)
    // glEnableVertexAttribArray(index)

    glBindVertexArray(0);

    // �`�揈��
    return 0;
}