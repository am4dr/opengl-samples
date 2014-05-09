#pragma comment(lib, "opengl32")
#include "../suika/suika.h"
#include <cmath>

int main(int argc, char **argv) {
    const int initialSize = 480;
    GLFWwindow *window =
        suika::glfw::initializeWindowAndContext(
        initialSize, initialSize, "transform feedback", nullptr, nullptr, true);
    glfwSetWindowSizeCallback(window, 
        [](GLFWwindow *window, int width, int height){
        int viewportSize = std::min(width, height);
        int widthPadding = std::max((width - viewportSize) / 2, 0);
        int heightPadding = std::max((height - viewportSize) / 2, 0);
        glViewport(widthPadding, heightPadding, viewportSize, viewportSize);
    });
    GLuint backgroundVAO, backgroundVBO;
    glGenVertexArrays(1, &backgroundVAO);
    glBindVertexArray(backgroundVAO);
    glGenBuffers(1, &backgroundVBO);
    glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
    GLfloat vertices[] = {
        0.9f, 0.9f, -0.9f, 0.9f, -0.9f, -0.9f, 0.9f, -0.9f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    std::vector<suika::shader::ShaderSource> backgroundSources;
    backgroundSources.push_back(suika::shader::ShaderSource(GL_VERTEX_SHADER, std::string(
        "#version 330 core\n"
        "layout(location=0) in vec2 position;\n"
        "\n"
        "void main(void) {\n"
        "   gl_Position = vec4(position, 0.0f, 1.0f);\n"
        "}\n"), "background vertex shader"));
    backgroundSources.push_back(suika::shader::ShaderSource(GL_FRAGMENT_SHADER, std::string(
        "#version 330 core\n"
        "layout(location=0) out vec4 color;"
        "void main(void) {"
        "   color = vec4(0.0f, 0.0f, 0.0f, 1.0f);"
        "}"), "background fragment shader"));
    GLuint backgroundProgram = suika::shader::createShaderProgram(backgroundSources);
    glUseProgram(backgroundProgram);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    GLuint particleVAO;
    glGenVertexArrays(1, &particleVAO);
    glBindVertexArray(particleVAO);
    GLuint particleVBO[2];
    GLuint &particlePositionVBO = particleVBO[0];
    GLuint &particleColorVBO = particleVBO[1];
    glGenBuffers(2, particleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, particlePositionVBO);
    const int numberOfParticles = 2 * initialSize * initialSize;
    auto particlePositions =
        std::unique_ptr<GLfloat[]>(new GLfloat[numberOfParticles]);
    for (int i = 0; i < initialSize; ++i) {
        float yValue = static_cast<float>(i) / initialSize - 0.5f;
        for (int j = 0; j < initialSize; ++j) {
            int particleNumber = i*initialSize + j;
            particlePositions[2*particleNumber] = 
                static_cast<float>(j) / initialSize - 0.5f;
            particlePositions[2 * particleNumber + 1] = yValue;
        }
    }
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat)*numberOfParticles, particlePositions.get(), GL_STATIC_DRAW);
    std::vector<suika::shader::ShaderSource> particleSources;
    particleSources.push_back(suika::shader::ShaderSource(GL_VERTEX_SHADER, std::string(
        "#version 330 core\n"
        "layout(location=0) in vec2 position;"
        "out vec2 newPosition;"
        "float step = 0.005f;"
        "void main(void) {"
        "   vec2 normal = normalize(position);"
        "   vec2 pos = position + step * normal;"
        "   if (pos.x > 1.0f || pos.x < -1.0f"
        "       || pos.y > 1.0f || pos.y < -1.0f) {"
        "       pos = step * normalize(pos);"
        "   }"
        "   newPosition = pos;"
        "   gl_Position = vec4(position, 0.0f, 1.0f);"
        "}"), "particle vertex shader"));
    particleSources.push_back(suika::shader::ShaderSource(GL_FRAGMENT_SHADER, std::string(
        "#version 330 core\n"
        "layout(location=0) out vec4 color;"
        "void main(void) {"
        "   color = vec4(0.8f, 0.8f, 0.8f, 1.0f);"
        "}"), "particle fragment shader"));
    GLuint particleProgram = suika::shader::createShaderProgram(particleSources);
    glUseProgram(particleProgram);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    GLuint feedback;
    glGenBuffers(1, &feedback);
    glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, feedback);
    glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER,
        sizeof(GLfloat)*numberOfParticles, nullptr, GL_DYNAMIC_COPY);
    const char *varyings[] = { "newPosition" };
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, feedback);
    glTransformFeedbackVaryings(particleProgram, 1, varyings, GL_INTERLEAVED_ATTRIBS);
    glLinkProgram(particleProgram);

    // TODO �J���[�����O
    //glBindBuffer(GL_ARRAY_BUFFER, particleColorVBO);
    //GLfloat particleColors[3 * initialSize*initialSize];
    glClearColor(0.15f,0.15f,0.15f,1.0f);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(backgroundProgram);
        glBindVertexArray(backgroundVAO);
        glDrawArrays(GL_QUADS, 0, 4);

        glUseProgram(particleProgram);
        glBindVertexArray(particleVAO);
        glBeginTransformFeedback(GL_POINTS);
        static bool flag = true;
        glDrawArrays(GL_POINTS, 0, initialSize*initialSize);
        glEndTransformFeedback();
        glCopyBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, GL_ARRAY_BUFFER,
            0, 0, sizeof(GLfloat)*numberOfParticles);

        glFinish();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
