//
// Created by lq200 on 2023/3/22.
//

#ifndef OPENGL_GAMES_MODULECIRCLE_H
#define OPENGL_GAMES_MODULECIRCLE_H

#include "../GLUtils.h"
#include "../GLModule.h"

class ModuleCircle : GLModule {
public:

    bool init() override;

    bool loop() override;

    void destroy() override;

private:
    GLuint program{}, frag{}, vert{};
    GLuint buffer{};
    GLuint vao{};
    GLint locRatio{};

    int acc = 4096;
    float r = 0.5f, ratio = 0;
};


#endif //OPENGL_GAMES_MODULECIRCLE_H
