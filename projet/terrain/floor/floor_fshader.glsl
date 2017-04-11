#version 330 core
out vec4 color;

in float height_modif;
in vec2 uv;
uniform sampler2D tex;

in vec3 light_dir;
in vec3 view_dir;
in vec4 vpoint_mv;

uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;
uniform float alpha;

void main() {
    vec3 normal_tr = normalize(cross(dFdx(vpoint_mv.xyz), dFdy(vpoint_mv.xyz)));

    vec3 nl = vec3(clamp(dot(normal_tr, light_dir), 0, 1));
    vec3 light = vec3(0.8, 0.8, 1);
    vec3 diffuse =  light * nl * Ld;

    color = vec4(0.0, 0.0, 0.9, 0.0);

    float height = texture(tex, uv).r ;
    if (height<height_modif) {
        color.a = 0.3;
    }
    vec3 reflectn = normalize(reflect(-light_dir, normal_tr));
    vec3 rv = vec3(pow(clamp(dot(reflectn, view_dir), 0, 1), alpha));
    vec3 specular = rv * Ls;
    //vec3 specular = vec3(0);
    color = color + vec4(diffuse, 0.0) + vec4(specular, 0.0);
}
