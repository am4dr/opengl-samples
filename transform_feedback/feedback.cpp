#include <suika.h>

int main(int argc, char **argv) {
    const int initialSize = 600;
    GLFWwindow *window =
        suika::glfw::initializeWindowAndContext(
        initialSize, initialSize, "transform feedback", nullptr, nullptr, true);
    glfwSetWindowSizeCallback(window, suika::glfw::centeredMaximizedSquareViewport);
    GLuint particleProgram = 
        suika::shader::makeProgram("transform_feedback.vert", "transform_feedback.frag");
    
    GLuint particlePositionVBO[2];
    glGenBuffers(2, particlePositionVBO);
    const int numberOfParticles = initialSize * initialSize;
    auto particlePositions =
        std::unique_ptr<GLfloat[]>(new GLfloat[2*numberOfParticles]);
    for (int i = 0; i < initialSize; ++i) {
        float yValue = static_cast<float>(i) / initialSize - 0.5f;
        for (int j = 0; j < initialSize; ++j) {
            int particleNumber = i*initialSize + j;
            particlePositions[2*particleNumber] = 
                static_cast<float>(j) / initialSize - 0.5f;
            particlePositions[2 * particleNumber + 1] = yValue;
        }
    }
    GLuint particleColorVBO;
    glGenBuffers(1, &particleColorVBO);
    auto particleColors =
        std::unique_ptr<GLfloat[]>(new GLfloat[numberOfParticles]);
    for (int i = 0; i < initialSize; ++i) {
        int y = i - initialSize / 2;
        for (int j = 0; j < initialSize; ++j) {
            int x = j - initialSize / 2;
            particleColors[i*initialSize + j] =
                4.0f * (x*x + y*y) / (initialSize*initialSize);
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, particlePositionVBO[0]);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * 2 * numberOfParticles, particlePositions.get(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, particlePositionVBO[1]);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * 2 * numberOfParticles, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, particleColorVBO);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * numberOfParticles, particleColors.get(), GL_STATIC_DRAW);

    GLuint particleVAO[2];
    glGenVertexArrays(2, particleVAO);
    for (int i = 0; i < 2; ++i) {
        glBindVertexArray(particleVAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, particlePositionVBO[i]);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, particleColorVBO);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }
    glBindVertexArray(0);

    const char *varyings[] = { "newPosition" };
    glTransformFeedbackVaryings(particleProgram, 1, varyings, GL_INTERLEAVED_ATTRIBS);
    glLinkProgram(particleProgram);

    glClearColor(0.15f,0.15f,0.15f,1.0f);
    glUseProgram(particleProgram);
    int bufferNumber = 0;
    while (!glfwWindowShouldClose(window)) {
        int nextBufferNumber = 1 - bufferNumber;
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(particleVAO[bufferNumber]);
        // 今回描画に使用しないほうのバッファにfeedbackする
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0,
            particlePositionVBO[nextBufferNumber]);
        glBeginTransformFeedback(GL_POINTS);
        glDrawArrays(GL_POINTS, 0, numberOfParticles);
        glEndTransformFeedback();

        glFinish();
        glfwSwapBuffers(window);
        glfwPollEvents();
        // 描画するバッファの切り替え
        bufferNumber = nextBufferNumber;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
