#version 330 core

out vec4 frag_color;

in VS_OUT {
    vec3 frag_pos;
    vec3 norm_vec;
    vec2 vert_tex;
} fs_in;

uniform sampler2D texture_floor;
uniform vec3 light_pos[4];
uniform vec3 light_color[4];
uniform vec3 view_pos;
uniform bool is_gamma;

vec3 blinn_phong(vec3 n_vec, vec3 f_pos, vec3 l_pos, vec3 l_clr) {
    // attenuation part
    float max_dist  = 1.5;
    float distance  = length(l_pos - f_pos);
    float atten_val = 1.0 / (is_gamma ? distance * distance : distance);

    // diffuse part
    vec3 light_dir   = normalize(l_pos - f_pos);
    float diff_val   = max(dot(light_dir, n_vec), 0);
    vec3 diffuse_vec = diff_val * l_clr * atten_val;

    // specular part
    vec3 view_dir    = normalize(view_pos - f_pos);
    vec3 refl_dir    = reflect(-light_dir, n_vec);
    vec3 halfway_dir = normalize(light_dir + view_dir);
    float spec_val   = pow(max(dot(n_vec, halfway_dir), 0), 64);
    vec3 spec_vec    = spec_val * l_clr * atten_val;

    return diffuse_vec + spec_vec;
}

void main() {
    vec3 color_vec = texture(texture_floor, fs_in.vert_tex).rgb;
    vec3 light_vec = vec3(0);
    for (int i = 0; i < 4; ++i)
        light_vec += blinn_phong(normalize(fs_in.norm_vec), fs_in.frag_pos,
                light_pos[i], light_color[i]);
    color_vec *= light_vec;
    if (is_gamma)
        color_vec = pow(color_vec, vec3(1.0 / 2.2));
    frag_color = vec4(color_vec, 1);
}
