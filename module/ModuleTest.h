//
// Created by lq200 on 2023/3/19.
//

#ifndef OPENGL_GAMES_MODULETEST_H
#define OPENGL_GAMES_MODULETEST_H

#include "../GLUtils.h"
#include "../GLModule.h"

class ModuleTest : GLModule {
public:
    bool init() override;

    bool loop() override;

    void destroy() override;

private:
    GLuint program = 0, vert = 0, frag = 0;
    GLuint bufVertices = 0;
    GLuint bufIndices = 0;
    GLuint vao = 0;

    float vertices[20] {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
    };
    unsigned int indices[6] {
        0, 1, 2, 2, 1, 3
    };
};

#endif //OPENGL_GAMES_MODULETEST_H
