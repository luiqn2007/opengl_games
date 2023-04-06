//
// Created by lq200 on 2023/3/19.
//

#include "ModuleTest.h"

bool ModuleTest::init() {
    glfwSetKeyCallback(gl::window::window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);
    });

    if (!gl::shader::createShader(ROOT_DIR / "glsl" / "test.vert", vert)) return false;
    if (!gl::shader::createShader(ROOT_DIR / "glsl" / "ball.frag", frag)) return false;
    if (!gl::shader::createProgram(program, 2, vert, frag)) return false;

    glCreateBuffers(1, &bufVertices);
    glCreateBuffers(1, &bufIndices);

    glBindBuffer(GL_ARRAY_BUFFER, bufVertices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIndices);
    glNamedBufferData(bufVertices, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glNamedBufferData(bufIndices, sizeof(indices), indices, GL_STATIC_DRAW);

    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, SIZE_VEC2 + SIZE_VEC3, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, SIZE_VEC2 + SIZE_VEC3, offset(8));

    glBindBuffer(GL_ARRAY_BUFFER, bufVertices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIndices);
    glUseProgram(program);
    glBindVertexArray(vao);
    return true;
}

bool ModuleTest::loop() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, offset(0));
    return true;
}

void ModuleTest::destroy() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &bufIndices);
    glDeleteBuffers(1, &bufVertices);

    glDeleteShader(vert);
    glDeleteShader(frag);

    glfwSetKeyCallback(gl::window::window, nullptr);
}
