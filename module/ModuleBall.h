//
// Created by lq200 on 2023/3/27.
//

#ifndef OPENGL_GAMES_MODULEBALL_H
#define OPENGL_GAMES_MODULEBALL_H

#include "../GLUtils.h"
#include "../GLModule.h"

#include "glm/vec2.hpp"

using std::vector;
using glm::vec2;

namespace module_ball {
    struct Ball {
        glm::vec2 pos, vec, acc;
    };

    class ModuleBall : GLModule {
    public:
        bool init() override;

        bool loop() override;

        void destroy() override;

    private:
        Ball balls[5][5];
        GLuint program, vao, vert, geom, frag, buffer;
        GLint prog_ratio;
        GLfloat ratio = 0;
    };
}

#endif //OPENGL_GAMES_MODULEBALL_H
