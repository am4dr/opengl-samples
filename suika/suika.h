#ifndef INCLUDE_GUARD_AMADARE_SUIKA_20140430
#define INCLUDE_GUARD_AMADARE_SUIKA_20140430
/*
参考:
    https://www.opengl.org/wiki_132/index.php?title=GLSL_Object&printable=yes
*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <memory>

namespace suika {
    namespace shader {
        // シェーダの種類とシェーダのソースとシェーダの名前を持つ型
        class ShaderSource {
            GLenum type;
            std::string source;
            std::string name;
        public:
            ShaderSource(GLenum type, const std::string &source,
                const std::string &name = "");
            GLenum getType() const;
            const std::string &getSource() const;
            const std::string &getName() const;
        };
        // シェーダのソースファイルからShaderSourceを作成する。
        ShaderSource readShaderSource(GLenum type, const std::string &filename,
            const std::string &name = "");
        // 複数のShaderSourceをコンパイルおよびリンクしてプログラムを作成する。
        GLuint createShaderProgram(const std::vector<ShaderSource> &sources);
        GLuint makeProgram(const std::string &vertexShaderFileName,
            const std::string &fragmentShaderFileName = "");
        std::shared_ptr<GLchar> getShaderInfoLog(GLuint shader);
        std::shared_ptr<GLchar> getProgramInfoLog(GLuint program);
    }
    // ファイルの内容をstringとして返す
    std::string readFile(const std::string &filename);
    namespace glfw {
        void error_callback(int, const char *);
        void centeredMaximizedSquareViewport(GLFWwindow *window, int width, int height);
        // ひとつのウィンドウとコンテキストを使用する際に記述を省略する関数
        // 引数debugにより、デバッグ用のコンテキストを作成するかを選択できる。
        // 次のことを行う。
        // - glfwへのエラーコールバックの登録
        // - glfwInit
        // - glfwCreateWindowでGLFWwindowの生成
        // - glfwMakeContextCurrentで生成したウィンドウのコンテキストをカレントに設定
        // - glewInit
        GLFWwindow *initializeWindowAndContext(int width, int height, const char* title,
            GLFWmonitor* monitor, GLFWwindow* share, bool debug=false);
    }
    namespace gl {
        void APIENTRY debug_message_callback(GLenum source, GLenum type,
            GLuint id, GLenum severity, GLsizei length, const GLchar* message,
            GLvoid* userParam = nullptr);
    }
}
#endif // INCLUDE_GUARD_AMADARE_SUIKA_20140430