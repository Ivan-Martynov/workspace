#version 450 core

out VS_OUT
{
    vec2 tex_coord;
} vs_out;

uniform mat4 transformation_matrix;
uniform float offset;

void main() {

    const vec2[4] rect_pos = vec2[4](
            vec2(-0.5, -0.5),
            vec2( 0.5, -0.5),
            vec2(-0.5,  0.5),
            vec2( 0.5,  0.5));

    vs_out.tex_coord = (rect_pos[gl_VertexID].xy + vec2(offset, 0.5)) *
        vec2(30, 1);

    gl_Position = transformation_matrix * vec4(rect_pos[gl_VertexID], 0, 1);
}

