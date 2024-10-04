#version 450 core

layout (location = 0) in int alien_index;

out VS_OUT {
    flat int alien;
    vec2 tex_coord;
} vs_out;

struct droplet_t {
    float x_offset;
    float y_offset;
    float orientation;
    float unused;
};

layout (std140) uniform droplets {
    droplet_t droplet[256];
};

void main() {

    const vec2[4] position = vec2[4](
            vec2(-0.5, -0.5),
            vec2( 0.5, -0.5),
            vec2(-0.5,  0.5),
            vec2( 0.5,  0.5));

    vs_out.tex_coord = position[gl_VertexID].xy + vec2(0.5);

    const float cos_component = cos(droplet[alien_index].orientation);
    const float sin_component = sin(droplet[alien_index].orientation);

    const mat2 rot_matrix = mat2(vec2(cos_component, sin_component),
            vec2(-sin_component, cos_component));

    const vec2 pos = 0.25 * rot_matrix * position[gl_VertexID];

    gl_Position = vec4(
            pos.x + droplet[alien_index].x_offset,
            pos.y + droplet[alien_index].y_offset, 0.5, 1);

    vs_out.alien = alien_index % 64;
}

