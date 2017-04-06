#version 330

in vec2 uv;
in vec3 light_dir;
in vec3 view_dir;
in vec4 vpoint_mv;

out vec3 color;

uniform sampler1D colormap;
uniform sampler2D tex;
uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;


void main() {
    vec3 normal_tr = normalize(cross(dFdx(vpoint_mv.xyz), dFdy(vpoint_mv.xyz)));

    vec3 nl = vec3(clamp(dot(normal_tr, light_dir), 0, 1));
    vec3 diffuse = kd * nl * Ld;
    diffuse = vec3(0);

    float height = (texture(tex, uv).r + 1)/2.0f;

    color = diffuse + texture(colormap, height).rgb;
}
