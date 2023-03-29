cmake_minimum_required(VERSION 3.23)
project(opengl_games)

include_directories(include)
link_directories(lib)

set(CMAKE_CXX_STANDARD 17)

add_executable(opengl_games src/glad.c main.cpp include/glm
        GLModule.h module/ModuleCircle.cpp module/ModuleCircle.h module/ModuleTest.cpp module/ModuleTest.h
        GLUtils.cpp GLUtils.h module/ModuleBall.cpp module/ModuleBall.h)

target_link_libraries(opengl_games glfw3 opengl32)
