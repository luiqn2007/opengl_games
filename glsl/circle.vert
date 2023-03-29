#version 460 core

uniform struct {
    vec2 center;
    int acc;
    float r;
    float ratio; // w / h
} circle;

layout(location = 0) in float pos;
layout(location = 1) in vec3 color;

out vec4 fColor;

const float PI = 3.1415926;

void main() {
    fColor = vec4(color, 1);
    if (pos == -1) {
        gl_Position = vec4(circle.center, 0, 1);
    } else {
        float ang = 2 * PI / circle.acc * pos;
        float dx, dy;
        if (circle.ratio < 1) {
            dx = circle.r * cos(ang) + circle.center.x;
            dy = circle.r * circle.ratio * sin(ang) + circle.center.y;
        } else {
            dx = circle.r / circle.ratio * cos(ang) + circle.center.x;
            dy = circle.r * sin(ang) + circle.center.y;
        }
        gl_Position = vec4(dx, dy, 0, 1);
    }
}
