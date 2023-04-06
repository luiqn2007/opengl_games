//
// Created by lq200 on 2023/3/22.
//

#include "ModuleCircle.h"

bool ModuleCircle::init() {
    using namespace gl::shader;

    if (!createShader(ROOT_DIR / "glsl" / "circle.vert", vert)) return false;
    if (!createShader(ROOT_DIR / "glsl" / "ball.frag", frag)) return false;
    if (!createProgram(program, 2, vert, frag)) return false;

    glUseProgram(program);
    glUniform2f(glGetUniformLocation(program, "circle.center"), 0, 0);
    glUniform1i(glGetUniformLocation(program, "circle.acc"), acc);
    glUniform1f(glGetUniformLocation(program, "circle.r"), r);
    locRatio = glGetUniformLocation(program, "circle.ratio");

    glCreateBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glNamedBufferStorage(buffer, (SIZE_FLOAT + SIZE_VEC3) * (acc + 2), offset(0), GL_MAP_WRITE_BIT | GL_MAP_READ_BIT);
    auto *pbuf = (float *) glMapNamedBufferRange(buffer, 0, (SIZE_INT + SIZE_VEC3) * (acc + 2), GL_MAP_WRITE_BIT);
    *(pbuf++) = -1;
    *(pbuf++) = 0;
    *(pbuf++) = 0;
    *(pbuf++) = 0;
    for (int i = 0; i <= acc; ++i) {
        *(pbuf++) = (float) i;
        *(pbuf++) = 1;
        *(pbuf++) = 1;
        *(pbuf++) = 1;
    }
    glUnmapNamedBuffer(buffer);

    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, SIZE_FLOAT + SIZE_VEC3, offset(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, SIZE_FLOAT + SIZE_VEC3, offset(SIZE_FLOAT));
    return true;
}

bool ModuleCircle::loop() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    if (ratio != gl::window::ratio) {
        ratio = gl::window::ratio;
        glUniform1f(locRatio, ratio);
    }

    glDrawArrays(GL_TRIANGLE_FAN, 0, acc + 2);
    return true;
}

void ModuleCircle::destroy() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

    glDeleteBuffers(1, &buffer);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);

    glDeleteShader(frag);
    glDeleteShader(vert);
}
