//
// Created by lq200 on 2023/3/27.
//

#include "ModuleBall.h"

namespace module_ball {

    bool ModuleBall::init() {
        using namespace gl::shader;

        for (int i = 1; i <= 5; ++i) {
            for (int j = 1; j <= 5; ++j) {
                auto &ball = balls[i - 1][j - 1];
                ball.pos.x = i * (2.0f / 6) - 1;
                ball.pos.y = j * (2.0f / 6) - 1;
                ball.vec.x = gl::random::nextFloat() * 0.1f;
                ball.vec.y = gl::random::nextFloat() * 0.1f;
                ball.acc.x = 0;
                ball.acc.y = 0;
            }
        }

        if (!createShader(ROOT_DIR / "glsl" / "ball.vert", vert)) return false;
        if (!createShader(ROOT_DIR / "glsl" / "ball.geom", geom)) return false;
        if (!createShader(ROOT_DIR / "glsl" / "color.frag", frag)) return false;
        if (!createProgram(program, 2, vert, geom, frag)) return false;
        glUseProgram(program);
        prog_ratio = glGetUniformLocation(program, "ratio");
        glUniform1f(glGetUniformLocation(program, "r"), 0.02f);

        glCreateBuffers(1, &buffer);
        glBindBuffer(GL_VERTEX_ARRAY, buffer);
        glNamedBufferStorage(buffer, 2 * SIZE_VEC2, offset(0), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * SIZE_VEC2, offset(0));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * SIZE_VEC2, offset(SIZE_VEC2));
        glVertexAttribDivisor(0, 1);
        glVertexAttribDivisor(1, 1);

        return true;
    }

    bool ModuleBall::loop() {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        if (ratio != gl::window::ratio) {
            ratio = gl::window::ratio;
            glUniform1f(prog_ratio, ratio);
        }

        glDrawArraysInstanced(GL_POINTS, 0, 1, 25);

        return true;
    }

    void ModuleBall::destroy() {
        glUseProgram(0);
        glDeleteProgram(program);
        glDeleteShader(vert);
        glDeleteShader(geom);
        glDeleteShader(frag);

        glBindVertexArray(0);
        glDeleteVertexArrays(1, &vao);

        glBindBuffer(GL_VERTEX_ARRAY, 0);
        glDeleteBuffers(1, &buffer);

        ratio = 0;
    }

}
