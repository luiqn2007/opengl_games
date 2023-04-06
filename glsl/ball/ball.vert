#version 460 core

layout(location = 0) in vec3 color;
layout(location = 1) in vec3 pos;

out vec3 gColor;

void main() {
    gColor = color;
    gl_Position = vec4(pos, 1);
}
