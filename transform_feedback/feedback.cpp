#pragma comment(lib, "opengl32")
#include "../suika/suika.h"
#include <cmath>

int main(int argc, char **argv) {
    const int initialSize = 600;
    GLFWwindow *window =
        suika::glfw::initializeWindowAndContext(
        initialSize, initialSize, "transform feedback", nullptr, nullptr, true);
    glfwSetWindowSizeCallback(window, suika::glfw::centeredMaximizedSquareViewport);
    GLuint particleProgram = 
        suika::shader::makeProgram("transform_feedback.vert", "transform_feedback.frag");
    
    GLuint particleVAO;
    glGenVertexArrays(1, &particleVAO);
    glBindVertexArray(particleVAO);
    GLuint particleVBO[2];
    const GLuint &particlePositionVBO = particleVBO[0];
    const GLuint &particleColorVBO = particleVBO[1];
    glGenBuffers(2, particleVBO);
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
    glBindBuffer(GL_ARRAY_BUFFER, particlePositionVBO);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * 2 * numberOfParticles, particlePositions.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, particleColorVBO);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * numberOfParticles, particleColors.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    GLuint feedback;
    glGenBuffers(1, &feedback);
    glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, feedback);
    glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER,
        sizeof(GLfloat)*2*numberOfParticles, nullptr, GL_STATIC_COPY);
    const char *varyings[] = { "newPosition" };
    glTransformFeedbackVaryings(particleProgram, 1, varyings, GL_INTERLEAVED_ATTRIBS);
    glLinkProgram(particleProgram);

    glClearColor(0.15f,0.15f,0.15f,1.0f);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(particleProgram);
        glBindVertexArray(particleVAO);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, feedback);
        glBeginTransformFeedback(GL_POINTS);
        static bool flag = true;
        glDrawArrays(GL_POINTS, 0, numberOfParticles);
        glEndTransformFeedback();
        glBindBuffer(GL_ARRAY_BUFFER, particlePositionVBO);
        glCopyBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, GL_ARRAY_BUFFER,
            0, 0, sizeof(GLfloat)*2*numberOfParticles);

        glFinish();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
