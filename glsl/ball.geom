#version 460 core

const int N = 128;

layout(points) in;
layout(triangle_strip, max_vertices=N) out;

uniform float r;
uniform float ratio; // w / h

in vec3 gColor[];

out vec4 fColor;

const float da = 3.1415926 * 2 / N;

void main() {
    fColor = vec4(gColor[0], 1);
    vec4 pos = gl_in[0].gl_Position;
    for (int i = 1; i <= N / 2; ++i) {
        int idx1 = N / 2 - i;
        int idx2 = N / 2 + i - 1;
        float ang1 = da * idx1 + da / 2;
        float ang2 = da * idx2 + da / 2;

        if (ratio < 1) {
            float dx = r * cos(ang1) + pos.x;
            float dy = r * ratio * sin(ang1) + pos.y;
            gl_Position = vec4(dx, dy, 0, 1);
            EmitVertex();

            dx = r * cos(ang2) + pos.x;
            dy = r * ratio * sin(ang2) + pos.y;
            gl_Position = vec4(dx, dy, 0, 1);
            EmitVertex();
        } else {
            float dx = r / ratio * cos(ang1) + pos.x;
            float dy = r * sin(ang1) + pos.y;
            gl_Position = vec4(dx, dy, 0, 1);
            EmitVertex();

            dx = r / ratio * cos(ang2) + pos.x;
            dy = r * sin(ang2) + pos.y;
            gl_Position = vec4(dx, dy, 0, 1);
            EmitVertex();
        }

        EndPrimitive();
    }
}