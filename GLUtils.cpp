//
// Created by lq200 on 2023/3/27.
//

#include "GLUtils.h"

namespace gl {

    namespace random {
        std::random_device seed;
        std::mt19937_64 engine{seed()};
        std::uniform_int_distribution<> distribution{0, 100};
    }

    namespace timer {
        float previous;
        float current;
        float delta;

        void update() {
            previous = current;
            current = glfwGetTime();
            delta = current - previous;
        }
    }

    namespace debug {
        void mapArray() {
            void *p = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
            float *pf = (float *) p;
            unsigned int *pui = (unsigned int *) p;
            glUnmapBuffer(GL_ARRAY_BUFFER);
        }

        void mapElement() {
            void *p = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);
            glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
        }

        void mapBuffer(GLuint buffer) {
            void *p = glMapNamedBuffer(buffer, GL_READ_ONLY);
            glUnmapNamedBuffer(buffer);
        }

        bool checkError(const char *name, bool logNoError) {
            GLenum error = glGetError();
            if (error == GL_NO_ERROR && !logNoError) {
                return true;
            }
            cout << name << ": ";
            switch (error) {
                case GL_NO_ERROR:
                    cout << "GL_NO_ERROR" << endl;
                    return true;
                case GL_INVALID_ENUM:
                    cout << "GL_INVALID_ENUM" << endl;
                    return false;
                case GL_INVALID_VALUE:
                    cout << "GL_INVALID_VALUE" << endl;
                    return false;
                case GL_INVALID_OPERATION:
                    cout << "GL_INVALID_OPERATION" << endl;
                    return false;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl;
                    return false;
                case GL_OUT_OF_MEMORY:
                    cout << "GL_OUT_OF_MEMORY" << endl;
                    return false;
                case GL_STACK_UNDERFLOW:
                    cout << "GL_STACK_UNDERFLOW" << endl;
                    return false;
                case GL_STACK_OVERFLOW:
                    cout << "GL_STACK_OVERFLOW" << endl;
                    return false;
                default:
                    cout << "Unknown error code " << error << endl;
                    return false;
            }
        }
    }

    namespace resource {
        std::filesystem::path ROOT_DIR;

        unique_ptr<char> readText(uintmax_t &len, const path &p, string &errlog) {
            if (!std::filesystem::is_regular_file(p)) {
                errlog += "File ";
                errlog += p.string();
                errlog += " is not a regular file.";
                return {nullptr};
            }
            len = file_size(p);
            char *src = new char[len + 1];
            len = 0;
            std::fstream fs;
            fs.open(p);
            while (!fs.eof()) src[len++] = (char) fs.get();
            src[len - 1] = '\0';
            fs.close();
            return unique_ptr<char>(src);
        }
    }

    namespace shader {
        bool createShader(GLenum type, const path &file, GLuint &shader) {
            shader = glCreateShader(type);
            int info;
            uintmax_t length;
            string errlog;
            auto src = resource::readText(length, file, errlog);
            if (!errlog.empty()) {
                cout << "Failed to create shader " << file.string() << endl;
                cout << "  " << errlog << endl;
                return false;
            }
            GLchar *psrc = src.get();
            glShaderSource(shader, 1, &psrc, reinterpret_cast<const GLint *>(&length));
            glCompileShader(shader);
            glGetShaderiv(shader, GL_COMPILE_STATUS, &info);
            if (info != GL_TRUE) {
                GLint len;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
                auto msg = new GLchar[len + 1];
                glGetShaderInfoLog(shader, len, &len, msg);
                glDeleteShader(shader);
                shader = 0;
                cout << "Failed to create shader " << file.string() << endl;
                cout << "  " << msg << endl;
                return false;
            }
            return true;
        }

        bool createShader(const path &file, GLuint &shader) {
            if (!file.has_extension()) {
                cout << "Not found extension name: " << file.string() << endl;
                return false;
            }
            auto ext = file.filename().extension().string();
            GLuint type;
            if (ext == ".vert") {
                type = GL_VERTEX_SHADER;
            } else if (ext == ".frag") {
                type = GL_FRAGMENT_SHADER;
            } else if (ext == ".geom") {
                type = GL_GEOMETRY_SHADER;
            } else {
                cout << "Unknown extension \"" << ext << "\": " << file.string() << endl;
                return false;
            }
            return createShader(type, file, shader);
        }

        bool createProgram(GLuint &program, unsigned int shaderCount, ...) {
            program = glCreateProgram();
            va_list va, vb;
            va_start(va, shaderCount);
            for (int i = 0; i < shaderCount; i++)
                glAttachShader(program, va_arg(va, GLuint));
            va_end(va);
            glLinkProgram(program);
            va_start(vb, shaderCount);
            for (int i = 0; i < shaderCount; i++)
                glDetachShader(program, va_arg(vb, GLuint));
            va_end(vb);
            int info;
            glGetProgramiv(program, GL_LINK_STATUS, &info);
            if (info != GL_TRUE) {
                GLint len;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
                auto msg = new GLchar[len + 1];
                glGetProgramInfoLog(program, len, &len, msg);
                glDeleteProgram(program);
                program = 0;
                cout << msg << endl;
                return false;
            }
            return true;
        }
    }

    namespace window {
        GLFWwindow *window = nullptr;
        int width = 0, height = 0;
        float ratio = 1;

        std::vector<std::function<void()>> onResized;

        void resize(int _width, int _height) {
            width = _width;
            height = _height;
            ratio = (float) _width / _height;

            for (const auto &item: onResized) item();
        }
    }

    namespace glmath {

        float distance2(const glm::vec2 &a, const glm::vec2 &b) {
            return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
        }
    }

    bool load() {
        using namespace std::filesystem;
        using resource::ROOT_DIR;

        ROOT_DIR = current_path();
        // clion: debug
        if (resource::ROOT_DIR.filename().string() == "cmake-build-debug") {
            ROOT_DIR = ROOT_DIR.parent_path();
        }

        // timer
        timer::current = glfwGetTime();
        timer::previous = timer::current;
        timer::delta = 0;
        return true;
    }

    void unload() {
    }
}
