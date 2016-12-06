#include <fstream>
#include <iostream>
#include <algorithm>
#include "suika.h"
using namespace std;

namespace suika {
    // �t�@�C���̓��e��string�Ƃ��ĕԂ�
    string readFile(const string &filename) {
        ifstream file(filename, ifstream::binary);
        if (file.fail()) {
            cerr << filename << ": �t�@�C���I�[�v�����s" << endl;
            // ���s���̏���
            throw "'" + filename + "'�̃I�[�v���Ɏ��s";
        }
        file.seekg(0, file.end);
        std::streamoff length = file.tellg();
        file.seekg(0, file.beg);

        char *source = new char[static_cast<string::size_type>(length)+1];
        file.read(source, length);
        source[length] = 0;
        string res(source);
        delete[] source;
        return res;
    }
    namespace glfw {
        void error_callback(int error, const char *description){
            cerr << description << endl;
        }
        void centeredMaximizedSquareViewport(GLFWwindow *window, int width, int height) {
            int viewportSize = min(width, height);
            int widthPadding = max((width - viewportSize) / 2, 0);
            int heightPadding = max((height - viewportSize) / 2, 0);
            glViewport(widthPadding, heightPadding, viewportSize, viewportSize);
        }
        GLFWwindow *initializeWindowAndContext(int width, int height,
            const char* title, GLFWmonitor* monitor, GLFWwindow* share, bool debug) {
            glfwSetErrorCallback(&suika::glfw::error_callback);
            glfwInit();
            if (debug) {
                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
            }
            GLFWwindow *window = glfwCreateWindow(width, height, title, monitor, share);
            glfwMakeContextCurrent(window);
            gl3wInit();
            if (debug) {
                glDebugMessageCallback(suika::gl::debug_message_callback, &std::cerr);
            }
            return window;
        }
    }
    namespace gl {
        void APIENTRY debug_message_callback(GLenum source, GLenum type, GLuint id,
            GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) {
            ostream &out =
                userParam == nullptr ?
                    std::cerr : 
                        *static_cast<ostream *>(const_cast<GLvoid *>(userParam));
            out << "message: " << message << std::endl;
        }
    }
    namespace shader {
        ShaderSource::ShaderSource(GLenum type, const string &source, const string &name)
            :type(type), source(source), name(name) {};
        GLenum ShaderSource::getType() const {
            return type;
        }
        const string &ShaderSource::getSource() const {
            return source;
        }
        const string &ShaderSource::getName() const {
            return name;
        }

        ShaderSource readShaderSource(GLenum type, const string &filename,
            const string &name) {
            string source = readFile(filename);
            return ShaderSource(type, source, name == "" ? filename : name);
        }
        GLuint createShaderProgram(const vector<ShaderSource> &sources) {
            vector<GLuint> shaders;
            for (const ShaderSource &source : sources) {
                GLuint shader = glCreateShader(source.getType());
                const GLchar *src = 
                    const_cast<const GLchar *>(source.getSource().c_str());
                glShaderSource(shader, 1, &src, nullptr);
                glCompileShader(shader);
                GLint compiledStatus;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledStatus);
                if (compiledStatus != GL_TRUE) {
                    auto log = getShaderInfoLog(shader);
                    cerr << "Shader compilation failed (" << source.getName() 
                         << "):\n" << log << endl;
                    return 0;
                }
                shaders.push_back(shader);
            }
            GLuint program = glCreateProgram();
            for (const auto& shader : shaders) {
                glAttachShader(program, shader);
            }
            glLinkProgram(program);
            GLint linkedStatus;
            glGetProgramiv(program, GL_LINK_STATUS, &linkedStatus);
            if (linkedStatus != GL_TRUE) {
                auto log = getProgramInfoLog(program);
                cerr << "Program linking failed:\n" << log << endl;
                for (const auto& shader : shaders) {
                    glDeleteShader(shader);
                }
                glDeleteProgram(program);
                return 0;
            }
            return program;
        }
        GLuint makeProgram(const string &vertexShaderFileName,
                           const string &fragmentShaderFileName) {
            if (vertexShaderFileName == "") {
                return 0;
            }
            vector<ShaderSource> shaderSources;
            shaderSources.push_back(readShaderSource(
                GL_VERTEX_SHADER, vertexShaderFileName));
            if (fragmentShaderFileName != "") {
                shaderSources.push_back(readShaderSource(
                    GL_FRAGMENT_SHADER, fragmentShaderFileName));
            }
            return createShaderProgram(shaderSources);
        }
        shared_ptr<GLchar> getShaderInfoLog(GLuint shader) {
            GLsizei logLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            shared_ptr<GLchar> log(new GLchar[logLength + 1], default_delete<GLchar[]>());
            glGetShaderInfoLog(shader, logLength, &logLength, log.get());
            return log;
        }
        shared_ptr<GLchar> getProgramInfoLog(GLuint program) {
            GLsizei logLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
            shared_ptr<GLchar> log(new GLchar[logLength + 1], default_delete<GLchar[]>());
            glGetProgramInfoLog(program, logLength, &logLength, log.get());
            return log;
        }
    }
}
