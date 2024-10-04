#version 450 core

layout (location = 0) in vec4 position;

uniform mat4 mvp;

out VS_OUT {
    vec4 pos;
    vec4 color;
} vs_out;

void main(void)
{
    const vec4 p = mvp * position;
    //const vec4 p = position;

    gl_Position = p;

    vs_out.pos = p / p.w;
    vs_out.color = vec4(1);
}

