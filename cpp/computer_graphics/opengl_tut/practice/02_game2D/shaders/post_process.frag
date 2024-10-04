#version 330 core

in vec2 tex_coords;

out vec4 color;

uniform sampler2D scene;
uniform vec2 offsets[9];
uniform int edge_kernel[9];
uniform float blur_kernel[9];

uniform bool is_chaos;
uniform bool is_confuse;
uniform bool is_shake;

void main() {
    color = vec4(0);
    vec3 sample[9];

    // sample from texture offsets using convolution matrix
    if (is_chaos || is_shake)
        for (int i = 0; i < 9; ++i)
            sample[i] = vec3(texture(scene, tex_coords.st + offsets[i]));

    // process effects
    if (is_chaos) {
        for (int i = 0; i < 9; ++i)
            color += vec4(sample[i] * edge_kernel[i], 0);
        color.a = 1;
    } else if (is_confuse) {
        color = vec4(1 - texture(scene, tex_coords).rgb, 1);
    } else if (is_shake) {
        for (int i = 0; i < 9; ++i)
            color += vec4(sample[i] * blur_kernel[i], 0);
        color.a = 1;
    } else {
        color = texture(scene, tex_coords);
    }
}

