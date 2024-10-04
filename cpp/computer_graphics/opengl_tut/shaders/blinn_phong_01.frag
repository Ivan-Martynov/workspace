#version 330 core

out vec4 frag_color;

in VS_OUT {
    vec3 frag_pos;
    vec3 norm_vec;
    vec2 vert_tex;
} fs_in;

uniform sampler2D texture_floor;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform bool is_blinn;

void main() {
    vec3 color_val = texture(texture_floor, fs_in.vert_tex).rgb;

    // ambient part
    vec3 amb_vec = 0.05 * color_val;

    // diffuse part
    vec3 light_dir = normalize(light_pos - fs_in.frag_pos);
    vec3 normal_vec = normalize(fs_in.norm_vec);
    float diff_val = max(dot(light_dir, normal_vec), 0);
    vec3 diffuse_vec = diff_val * color_val;

    // specular part
    vec3 view_dir = normalize(view_pos - fs_in.frag_pos);
    float spec_val = 0;
    if (is_blinn) {
        vec3 halfway_dir = normalize(light_dir + view_dir);
        spec_val = pow(max(dot(normal_vec, halfway_dir), 0), 32);
    } else {
        vec3 refl_dir = reflect(-light_dir, normal_vec);
        spec_val = pow(max(dot(view_dir, refl_dir), 0), 8);
    }
    vec3 spec_vec = vec3(0.3) * spec_val; // assume bright white light color

    // combined
    frag_color = vec4(amb_vec + diffuse_vec + spec_vec, 1);
}
