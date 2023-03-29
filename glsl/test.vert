#version 460

layout(location = 0) in vec2 pos;
layout(location = 1) in vec3 color;

out vec4 fColor;

void main() {
    gl_Position = vec4(pos, 0, 1);
    fColor = vec4(color, 1);
}
