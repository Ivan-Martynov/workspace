#version 330 core

out vec4 frag_color;

in VS_OUT {
    vec3 frag_pos;
    vec3 norm_vec;
    vec2 vert_tex;
    vec4 frag_pos_light_space;
} fs_in;

uniform sampler2D diffuse_tex;
uniform sampler2D shadow_map;

uniform vec3 light_pos;
uniform vec3 view_pos;

float calculate_shadow(vec4 light_frag_pos) {
    // perform perspective division
    vec3 proj_coords = light_frag_pos.xyz / light_frag_pos.w;
    proj_coords = proj_coords * 0.5 + 0.5;

    // closest depth value
    float closest_depth = texture(shadow_map, proj_coords.xy).r;
    // current depth value
    float current_depth = proj_coords.z;

    return (current_depth > closest_depth) ? 1.0f : 0.0f;
}

void main() {
    vec3 color_val = texture(diffuse_tex, fs_in.vert_tex).rgb;
    vec3 normal_vec = normalize(fs_in.norm_vec);
    vec3 light_clr = vec3(1);

    // ambient part
    vec3 amb_vec = 0.15 * color_val;

    // diffuse part
    vec3 light_dir = normalize(light_pos - fs_in.frag_pos);
    float diff_val = max(dot(light_dir, normal_vec), 0);
    vec3 diffuse_vec = diff_val * light_clr;

    // specular part
    vec3 view_dir = normalize(view_pos - fs_in.frag_pos);
    vec3 halfway_dir = normalize(light_dir + view_dir);
    vec3 spec_vec = pow(max(dot(normal_vec, halfway_dir), 0), 64) * light_clr;

    // shadow part
    float shadow_val = calculate_shadow(fs_in.frag_pos_light_space);
    vec3 shadow_vec = amb_vec + (1 - shadow_val) * (diffuse_vec + spec_vec);

    // final output
    frag_color = vec4(shadow_vec * color_val, 1);
}
