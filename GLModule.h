//
// Created by lq200 on 2023/3/17.
//

#ifndef OPENGL_GAMES_GLMODULE_H
#define OPENGL_GAMES_GLMODULE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using gl::resource::ROOT_DIR;

class GLModule {
public:
    virtual bool init() = 0;

    virtual bool loop() = 0;

    virtual void destroy() = 0;
};


#endif //OPENGL_GAMES_GLMODULE_H
