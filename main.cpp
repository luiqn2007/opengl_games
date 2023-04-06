#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLUtils.h"
#include "module/ModuleBall.h"

using std::cin;
using std::cout;
using std::endl;

using MT = module_ball::ModuleBall;

void onGLFWError(int error_code, const char *description) {
    cout << "ERROR: GLFW Error " << error_code << ": " << description << endl;
}

GLFWwindow *createWindow(int width = 800, int height = 600, const char *title = "Hello OpenGL",
                         GLFWmonitor *monitor = nullptr, GLFWwindow *share = nullptr) {
    gl::window::resize(width, height);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(width, height, title, monitor, share);
    gl::window::window = window;
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cout << "Failed to initGLEnv GLAD";
        return nullptr;
    }
    glfwSwapInterval(1);
    glfwShowWindow(window);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *, int w, int h) {
        glViewport(0, 0, w, h);
        gl::window::resize(w, h);
    });
    glViewport(0, 0, width, height);
    gl::window::resize(width, height);
    return window;
}

int cleanAndExit(int code, MT *m = nullptr) {
    if (m) m->destroy();
    if (gl::window::window) {
        glfwDestroyWindow(gl::window::window);
        gl::window::window = nullptr;
    }
    glfwTerminate();
    gl::unload();
    return code;
}

int main() {
    if (!glfwInit()) {
        cout << "Failed to initGLEnv GLFW";
        return 0;
    }
    glfwSetErrorCallback(onGLFWError);

    GLFWwindow *window = createWindow();
    if (!window) {
        cout << "Failed to createShaders GLFWwindow";
        return -1;
    }
    if (!gl::load()) return cleanAndExit(-1);

    MT module;

    if (!module.init()) return cleanAndExit(-1, &module);
    gl::window::resize(gl::window::width, gl::window::height);
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        gl::timer::update();
        if (!module.loop())
            return cleanAndExit(-1, &module);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return cleanAndExit(0, &module);
}
