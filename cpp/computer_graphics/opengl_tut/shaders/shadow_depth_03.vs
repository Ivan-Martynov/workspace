#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

out VS_OUT {
    vec3 frag_pos;
    vec3 norm_vec;
    vec2 vert_tex;
    vec4 frag_pos_light_space;
} vs_out;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform mat4 light_matrix;

void main() {
    gl_Position = proj * view * model * vec4(position, 1);
    vs_out.frag_pos = vec3(model * vec4(position, 1));
    vs_out.norm_vec = transpose(inverse(mat3(model))) * normal;
    vs_out.vert_tex = tex_coord;
    vs_out.frag_pos_light_space = light_matrix * vec4(vs_out.frag_pos, 1);
}
