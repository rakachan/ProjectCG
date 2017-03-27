#version 330

in vec2 uv;

layout (location = 0) out vec3 color;
layout (location = 1) out vec3 pass;
layout (location = 2) out vec3 blur;

uniform sampler2D tex;
uniform sampler2D tmp_tex;
uniform sampler2D tmp2_tex;

uniform float tex_width;
uniform float tex_height;
uniform int width_kernel;
uniform int max_size;
uniform float one_kernel[225];

float rgb_2_luma(vec3 c) {
    return 0.3*c[0] + 0.59*c[1] + 0.11*c[2];
}

void main() {
    bool one_pass = false;

#if 0

    if(one_pass) {
        float tot_weight = 0;
        vec3 tot_image = vec3(0);
        for(int i = 0; i < width_kernel; i++) {
            for(int j = 0; j < width_kernel; j++) {
                float weight = one_kernel[i * width_kernel + j];
                vec3 image = texture(tex, uv + vec2((i- width_kernel/2 -1)/tex_width, (j-width_kernel/2-1)/tex_height)).rgb;
                tot_image += weight * image;
                tot_weight += weight;
            }
        }
        color = tot_image/tot_weight;
    } else {
        float t_00 = rgb_2_luma( textureOffset(tex, uv, ivec2(-1, -1)).rgb );
        float t_01 = rgb_2_luma( textureOffset(tex, uv, ivec2(-1,  0)).rgb );
        float t_02 = rgb_2_luma( textureOffset(tex, uv, ivec2(-1, +1)).rgb );
        // x=0
        float t_10 = rgb_2_luma( textureOffset(tex, uv, ivec2( 0, -1)).rgb );
        float t_12 = rgb_2_luma( textureOffset(tex, uv, ivec2( 0, +1)).rgb );
        // x=+1
        float t_20 = rgb_2_luma( textureOffset(tex, uv, ivec2(+1, -1)).rgb );
        float t_21 = rgb_2_luma( textureOffset(tex, uv, ivec2(+1,  0)).rgb );
        float t_22 = rgb_2_luma( textureOffset(tex, uv, ivec2(+1, +1)).rgb );
        // gradients
        float sx = -1*t_00 -2*t_01 -1*t_02    +1*t_20 +2*t_21 +1*t_22;
        float sy = -1*t_00 -2*t_10 -1*t_20    +1*t_02 +2*t_12 +1*t_22;
        float g = 1-sqrt(sx*sx + sy*sy);

        color = vec3(g);
    }

#else
    vec3 G = vec3(1, 2, 1);
    //color = texture(tex, uv).rgb;

    pass = vec3(0.0);
    for(int i = -1; i <= 1; ++i) {
         pass += G[i + 1] * texture(tex, uv + vec2(i/tex_width,0.0)).rgb;
    }

    //color = vec3(0.0);
    //blur = texture(tmp_tex, uv).rgb;

    for(int i = -1; i <= 1; ++i) {
        color += G[i + 1] * texture(tmp_tex, uv + vec2(0.0, i/tex_height)).rgb;
    }

    blur = texture(tex, uv).rgb;


#endif
}

