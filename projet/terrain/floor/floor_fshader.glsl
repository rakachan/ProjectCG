#version 330 core
out vec4 color;

in float height_modif;
in vec2 uv;
uniform sampler2D tex;
uniform sampler2D ref;
in vec3 light_dir;
in vec3 view_dir;
in vec4 vpoint_mv;

uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;
uniform float alpha;

void main() {
    vec3 normal_tr = normalize(cross(dFdx(vpoint_mv.xyz), dFdy(vpoint_mv.xyz)));

    vec3 nl = vec3(clamp(dot(normal_tr, light_dir), 0, 1));
    vec3 diffuse =  kd * nl * Ld;

    color = vec4(0.0, 0.0, 0.9, 0.0);
    float height = texture(tex, uv).r +0.2 ;

    if (height<height_modif) {
        ivec2 dim = textureSize(ref, 0);
        float window_width = dim.x;
        float window_height = dim.y;
        vec2 coord = vec2((gl_FragCoord.x/(window_width)), (gl_FragCoord.y/(window_height)));

        vec3 reflectn = normalize(reflect(-light_dir, normal_tr));
        vec3 rv = vec3(pow(clamp(dot(reflectn, view_dir), 0, 1), alpha));
        vec3 specular = ks* rv * Ls;
        float view_angle = acos(dot(normal_tr, view_dir));

        color = mix(color+ vec4(diffuse, 0) + vec4(specular, 0), vec4(texture(ref, coord).rgb, 0.7), view_angle) ;
    } else {
        discard;
    }

}
