#include <suika.h>

// ����`�悷�锪�ʑ̗p�̒��_���W�ƃC���f�b�N�X�p�̃o�b�t�@���쐬���Ė��O��Ԃ��֐�
// restartMarker��primitive restart�p�̒l��ݒ肷��B
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
        0, 1, 2, 3, 4, 1, restartMarker, 5, 4, 3, 2, 1, 4
    };
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices), indices, GL_STATIC_DRAW);
}
// 3������ԏ�̊i�q�̌�_�̍��W�����ɂ߂��z���Ԃ��֐�
// ���W�̊e������[offset, max + offset]�̒l���Ƃ�B
// size�͊i�q�̊e�ӂ̕������Bsize��3��̍��W��������B
// max�͊i�q�̊e�ӂ̒����B
// offset�ɒl��ݒ肷�邱�Ƃœ�������W�̊e������offset���̒l�����Z����B
//     x=y=z�̒����̕�����offset�����s�ړ�����B
std::unique_ptr<GLfloat[]> create3dData(const int size, const GLfloat max, const GLfloat offset = 0.0f) {
    const int number = size * size * size;
    std::unique_ptr<GLfloat[]> data(new GLfloat[number * 3]);
    const GLfloat step = max / (size + 1);
    std::unique_ptr<GLfloat[]> ruler(new GLfloat[size]);
    for (int i = 0; i < size; ++i) {
        ruler[i] = step * (i + 1) + offset;
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                data[(i*size*size + j*size + k) * 3 + 0] = ruler[k];
                data[(i*size*size + j*size + k) * 3 + 1] = ruler[j];
                data[(i*size*size + j*size + k) * 3 + 2] = ruler[i];
            }
        }
    }
    return data;
}
// �C���X�^���X���Ƃɐݒ肷����W�ƐF���i�[����o�b�t�@���쐬���Ė��O��Ԃ��֐�
// ���W�͋�Ԃ̒��S����̑��Έʒu�B
void createInstanceData(GLuint &positionsVBO, GLuint &colorsVBO, const int size) {
    const int number = size * size * size;
    GLuint vbo[2];
    glGenBuffers(2, vbo);
    positionsVBO = vbo[0];
    colorsVBO = vbo[1];
    auto positions = create3dData(size, 2.0f, -1.0f);
    glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * number * 3, positions.get(), GL_STATIC_DRAW);

    auto colors = create3dData(size, 1.0f);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * number * 3, colors.get(), GL_STATIC_DRAW);
}
int main(int argc, char **argv) {
    GLFWwindow *window =
        suika::glfw::initializeWindowAndContext(
        600, 600, "instancing", nullptr, nullptr, true);
    glfwSetWindowSizeCallback(window, suika::glfw::centeredMaximizedSquareViewport);
    GLuint shaderProgram = suika::shader::makeProgram("instancing_vert.glsl", "instancing_frag.glsl");
    // �C���X�^���X�����郂�f���̃f�[�^���o�b�t�@�ɓ����
    GLuint modelVerticesBuffer;
    GLuint modelElementsIndicesBuffer;
    const GLubyte primitiveRestartIndex = 0xff;
    createModelData(modelVerticesBuffer, modelElementsIndicesBuffer, primitiveRestartIndex);
    // �C���X�^���X���Ƃɐݒ肷��f�[�^���o�b�t�@�ɓ����
    GLuint instancePositionBuffer;
    GLuint instanceColorBuffer;
    const int instancingSize = 5;
    const int instanceNumber = instancingSize * instancingSize * instancingSize;
    createInstanceData(
        instancePositionBuffer, instanceColorBuffer, instancingSize);
    // �v���O��������attribute�ϐ��̈ʒu���擾����
    GLint positionLocation = glGetAttribLocation(shaderProgram, "position");
    GLint instancePositionLocation = glGetAttribLocation(shaderProgram, "instancePosition");
    GLint instanceColorLocation = glGetAttribLocation(shaderProgram, "instanceColor");
    // �`��ɕK�v�ȃf�[�^��VAO�ɐݒ肷��
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, modelVerticesBuffer);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(positionLocation);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelElementsIndicesBuffer);
    // �C���X�^���X���Ƃ�Attrib�ݒ�
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
    // �`�揈��
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PRIMITIVE_RESTART);
    glEnable(GL_CULL_FACE);
    glPrimitiveRestartIndex(primitiveRestartIndex);
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElementsInstanced(GL_TRIANGLE_FAN, 13, GL_UNSIGNED_BYTE, 0, instanceNumber);
        glFinish();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
