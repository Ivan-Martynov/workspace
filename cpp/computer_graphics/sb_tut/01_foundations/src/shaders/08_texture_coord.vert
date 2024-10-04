#version 450 core

uniform mat4 modelview_matrix;
uniform mat4 projection_matrix;

layout (location = 0) in vec4 position;
layout (location = 4) in vec2 tex_coord;

out VS_OUT
{
    vec2 tex_coord;
} vs_out;

void main() {

    vec4 pos_vs = modelview_matrix * position;

    vs_out.tex_coord = tex_coord;

    gl_Position = projection_matrix * pos_vs;
    //gl_Position = projection_matrix * modelview_matrix * position;
}

