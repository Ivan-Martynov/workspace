#version 450 core

layout (location = 0) in vec4 offset;

out VS_OUT
{
    vec4 color;
} vs_out;

void main() {
    const vec4 vertices[] = vec4[] (
            vec4(0.25, -0.25, 0.5, 1),
            vec4(-0.25, -0.25, 0.5, 1),
            vec4(0.25, 0.25, 0.5, 1));

    const vec4 colors[] = vec4[] (
            vec4(1, 0, 0, 1),
            vec4(0, 1, 0, 1),
            vec4(0, 0, 1, 1));

    gl_Position = vertices[gl_VertexID];
    vs_out.color = colors[gl_VertexID];
}

