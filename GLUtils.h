//
// Created by lq200 on 2023/3/27.
//

#ifndef OPENGL_GAMES_GLUTILS_H
#define OPENGL_GAMES_GLUTILS_H

#define SIZE_FLOAT sizeof(float)
#define SIZE_INT sizeof(int)
#define SIZE_VEC2 (2 * sizeof(float))
#define SIZE_VEC3 (3 * sizeof(float))
#define SIZE_VEC4 (4 * sizeof(float))
#define SIZE_MAT3 (9 * sizeof(float))
#define SIZE_MAT4 (16 * sizeof(float))

#define offset(x) (reinterpret_cast<const void *>(x))

#include <iostream>
#include <memory>
#include <filesystem>
#include <fstream>
#include <map>
#include <cstdarg>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using std::cout;
using std::endl;
using std::map;
using std::string;
using std::unique_ptr;
using std::filesystem::path;
using std::mersenne_twister_engine;

namespace gl {

    namespace random {
        extern std::random_device seed;
        extern std::mt19937_64 engine;
        extern std::uniform_int_distribution<> distribution;

        inline int nextInt() {
            return distribution(engine);
        }

        inline float nextFloat() {
            return (float) nextInt() / 100.0f;
        }
    }

    namespace timer {
        void init();

        void update();

        void delta();
    }

    namespace error {

    }

    namespace debug {
        void mapArray();

        void mapElement();

        void mapBuffer(GLuint buffer);
    }

    namespace resource {
        extern path ROOT_DIR;

        unique_ptr<char> readText(uintmax_t &len, const path &p, string &errlog);
    }

    namespace shader {
        bool createShader(GLenum type, const path &file, GLuint &shader);

        bool createShader(const path &file, GLuint &shader);

        bool createProgram(GLuint &program, unsigned int shaderCount, ...);
    }

    namespace window {
        extern GLFWwindow *window;
        extern int width, height;
        extern float ratio;

        void resize(int width, int height);
    }

    bool load();

    void unload();
}

#endif //OPENGL_GAMES_GLUTILS_H
