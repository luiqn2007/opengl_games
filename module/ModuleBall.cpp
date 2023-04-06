//
// Created by lq200 on 2023/3/27.
//

#include "ModuleBall.h"

namespace module_ball {

    void Ball::updateMove(float p) {
        // position
        pos += vec * (gl::timer::delta * p);
        left = pos.x - radius;
        right = pos.x + radius;
        top = pos.y + radius;
        bottom = pos.y - radius;

        // direction
        if (left <= -1) {
            vec.x = abs(vec.x);
            left = -2 - left;
            pos.x = left + radius;
            right = pos.x + radius;
        } else if (right >= 1) {
            vec.x = -abs(vec.x);
            right = 2 - right;
            pos.x = right - radius;
            left = pos.x - radius;
        }

        if (bottom <= -1) {
            vec.y = abs(vec.y);
            bottom = -2 - bottom;
            pos.y = bottom + radius;
            top = pos.y + radius;
        } else if (top >= 1) {
            vec.y = -abs(vec.y);
            top = 2 - top;
            pos.y = top - radius;
            bottom = pos.y - radius;
        }
    }

    bool ModuleBall::init() {
        gl::window::onResized.push_back(resized);
        glEnable(GL_PROGRAM_POINT_SIZE);

        using namespace gl::shader;

        if (!createShader(ROOT_DIR / "glsl" / "ball" / "ball.vert", vert)) return false;
        if (!createShader(ROOT_DIR / "glsl" / "ball" / "ball.geom", geom)) return false;
        if (!createShader(ROOT_DIR / "glsl" / "ball" / "ball.frag", frag)) return false;
        if (!createProgram(program, 3, vert, geom, frag)) return false;
//        if (!createShader(ROOT_DIR / "glsl" / "test" / "ball.vert", vert)) return false;
//        if (!createShader(ROOT_DIR / "glsl" / "test" / "ball.frag", frag)) return false;
//        if (!createProgram(program, 2, vert, frag)) return false;
        glGetError();
        glUseProgram(program);
        prog_ratio = glGetUniformLocation(program, "ratio");

        glCreateBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glNamedBufferStorage(buffer, (SIZE_VEC3 + SIZE_VEC3) * 25, offset(0), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

        auto *bufColor = (float *) glMapNamedBufferRange(buffer, 0, SIZE_VEC3 * 25, GL_MAP_WRITE_BIT);
        for (int i = 1; i <= 5; ++i) {
            for (int j = 1; j <= 5; ++j) {
                auto &ball = balls[(i - 1) * 5 + j - 1];
                ball.pos.x = i * (2.0f / 6) - 1;
                ball.pos.y = j * (2.0f / 6) - 1;
                ball.radius = 0.1f;
                ball.updateMove(0);
                ball.vec.x = gl::random::nextFloat() * 0.1f;
                ball.vec.y = gl::random::nextFloat() * 0.1f;
                ball.acc.x = 0;
                ball.acc.y = 0;
                ball.color.x = gl::random::nextFloat();
                ball.color.y = gl::random::nextFloat();
                ball.color.z = gl::random::nextFloat();
                *bufColor++ = ball.color.x;
                *bufColor++ = ball.color.y;
                *bufColor++ = ball.color.z;
            }
        }
        glUnmapNamedBuffer(buffer);

        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, offset(0));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, offset(SIZE_VEC3 * 25));
        glVertexAttribDivisor(0, 1);
        glVertexAttribDivisor(1, 1);

        return true;
    }

    bool ModuleBall::loop() {
        glClearColor(0.5f, 0.5f, 0.5f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        updateBalls();
        auto *bufPos = (float *) glMapNamedBufferRange(buffer, SIZE_VEC3 * 25, SIZE_VEC3 * 25, GL_MAP_WRITE_BIT);
        for (auto &ball: balls) {
            *bufPos++ = ball.pos.x;
            *bufPos++ = ball.pos.y;
            *bufPos++ = ball.radius;
        }
        glUnmapNamedBuffer(buffer);

        glDrawArraysInstanced(GL_POINTS, 0, 1, 25);
        return true;
    }

    void ModuleBall::destroy() {
        gl::window::onResized.clear();

        glUseProgram(0);
        glDeleteProgram(program);
        program = 0;
        glDeleteShader(vert);
        glDeleteShader(geom);
        glDeleteShader(frag);

        glBindVertexArray(0);
        glDeleteVertexArrays(1, &vao);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &buffer);

        ratio = 0;
    }

    void ModuleBall::resize() {
        if (program) {
            ratio = gl::window::ratio;
            glUniform1f(prog_ratio, ratio);
        }
    }

    void ModuleBall::updateBalls() {
        for (auto &ball: balls) ball.updateMove(20);
    }
}
