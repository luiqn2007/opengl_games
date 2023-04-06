//
// Created by lq200 on 2023/3/27.
//

#ifndef OPENGL_GAMES_MODULEBALL_H
#define OPENGL_GAMES_MODULEBALL_H

#include "../GLUtils.h"
#include "../GLModule.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

using std::vector;
using glm::vec2;
using glm::vec3;

namespace module_ball {

    struct Ball {
        vec2 pos, vec, acc;
        vec3 color;
        float radius;
        float left, right, top, bottom;

        void updateMove(float p);
    };

    class ModuleBall : GLModule {
    public:
        bool init() override;

        bool loop() override;

        void destroy() override;

    private:
        GLuint program{}, vao{}, vert{}, geom{}, frag{}, buffer{};
        GLint prog_ratio{};
        GLfloat ratio = 0;
        Ball balls[25]{};
        std::function<void()> resized = [this]() { resize(); };

        void resize();

        void updateBalls();
    };
}

#endif //OPENGL_GAMES_MODULEBALL_H
