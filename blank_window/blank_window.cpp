#include <iostream>
#include <GLFW/glfw3.h>

int main(int argc, char** argv) {
    glfwSetErrorCallback([](int error, const char *description){
        std::cerr << description << std::endl;
    });
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    GLFWwindow *window;
    window = glfwCreateWindow(480, 480, "blank window", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, 
        [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    });

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
