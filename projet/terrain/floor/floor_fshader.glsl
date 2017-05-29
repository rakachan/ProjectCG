#version 330 core
out vec4 color;
uniform float time;
uniform vec3 light_pos;
uniform vec3 cam_pos;

in float height_modif;
in vec2 uv;
uniform sampler2D tex;
uniform sampler2D ref;
uniform sampler2D normalmap;

in vec4 vpoint_mv;
in mat4 MV;
in vec3 pos_3d;

uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;
uniform float alpha;

void main() {
    float height = texture(tex, uv).r +0.2 ;

    if (height<0) {
        vec3 light_dir = normalize(pos_3d - light_pos);
        vec3  view_dir = normalize(pos_3d - cam_pos);
        //vec3 normal = normalize(cross(dFdx(vpoint_mv.xyz), dFdy(vpoint_mv.xyz)));
        vec3 normal = normalize(texture(normalmap, 10.0f*uv+vec2((0.07f*(time+0.1f*sin(time)*sin(time)))), -0.1f*time).rgb);
        vec3 nl = vec3(clamp(dot(normal, light_dir), 0, 1));
        vec3 diffuse =  kd * nl * Ld;
        ivec2 dim = textureSize(ref, 0);
        float window_width = dim.x;
        float window_height = dim.y;
        vec2 coord = vec2((gl_FragCoord.x/(window_width)), (gl_FragCoord.y/(window_height)));

        vec3 reflectn = normalize(reflect(-light_dir, normal));
        vec3 rv = vec3(pow(clamp(dot(reflectn, view_dir), 0, 1), alpha));
        vec3 specular = ks * rv * Ls;
        float cos_angle = abs(dot(normal, view_dir));
        vec2 distortion = coord*height*0.3f*normal.xz;
        color =  vec4(diffuse, 0) + vec4(specular, 0) + vec4(texture(ref, coord+distortion).rgb, 0);
        color.a = mix(0.8, 0.5, cos_angle);
        //color = vec4(normal,1);
    } else {
        discard;
    }
}
