#version 330

in vec2 uv;

in vec3 light_dir;
in vec3 view_dir;
in vec4 vpoint_mv;
uniform sampler1D colormap;

uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;
uniform float alpha;

out vec3 color;

uniform sampler2D tex;
uniform sampler2D heightmap;



void main() {

    vec3 normal_tr = normalize(cross(dFdx(vpoint_mv.xyz), dFdy(vpoint_mv.xyz)));

    vec3 nl = vec3(clamp(dot(normal_tr, light_dir), 0, 1));
    vec3 diffuse = 0.3* nl * Ld;

    float height = (texture(tex, uv).r + 1)/2.0f;

    color = diffuse + texture(colormap, height).rgb;
}
