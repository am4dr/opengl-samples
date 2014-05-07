#include <fstream>
#include <iostream>
#include "suika.h"
using namespace std;

namespace suika {
    // ファイルの内容をstringとして返す
    string readFile(const string &filename) {
        ifstream file(filename, ifstream::binary);
        if (file.fail()) {
            cerr << filename << ": ファイルオープン失敗" << endl;
            // 失敗時の処理
            throw "'" + filename + "'のオープンに失敗";
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
        void initGLFW() {
            glfwSetErrorCallback(&suika::glfw::error_callback);
            if (!glfwInit()) {
                exit(EXIT_FAILURE);
            }
        }
        void error_callback(int error, const char *description){
            cerr << description << endl;
        }
    }
    namespace gl {
        void APIENTRY debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam) {
            ostream &out = userParam != nullptr ? *static_cast<ostream *>(userParam) : std::cerr;
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

        ShaderSource readShaderSource(GLenum type, const string &filename, const string &name) {
            string source = readFile(filename);
            return ShaderSource(type, source, name == "" ? filename : name);
        }
        GLuint createShaderProgram(const vector<ShaderSource> &sources) {
            vector<GLuint> shaders;
            for (const ShaderSource &source : sources) {
                GLuint shader = glCreateShader(source.getType());
                const GLchar *src = const_cast<const GLchar *>(source.getSource().c_str());
                glShaderSource(shader, 1, &src, nullptr);
                glCompileShader(shader);
                GLint compiledStatus;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledStatus);
                if (compiledStatus != GL_TRUE) {
                    auto log = getShaderInfoLog(shader);
                    cerr << "Shader compilation failed (" << source.getName() << "):\n" << log << endl;
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
