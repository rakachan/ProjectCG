#version 330

in vec2 uv;

in vec3 light_dir;
in vec3 view_dir;
in vec4 vpoint_mv;
in float height;

uniform sampler1D colormap;
uniform int draw;
uniform float time;

uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;
uniform float alpha;

out vec4 color;

uniform sampler2D tex;
//uniform sampler2D heightmap;



void main() {

    if ((draw==0)) {
        float height_modif = 0.02*(sin(time)*sin(time+uv.x*20)*sin(time+uv.y*20));
        if (height-abs(height_modif)>0) {
            discard;
        }
    }
    vec3 normal_tr = normalize(cross(dFdx(vpoint_mv.xyz), dFdy(vpoint_mv.xyz)));
    vec3 nl = vec3(clamp(dot(normal_tr, light_dir), 0, 1));
    vec3 diffuse = 0.3* nl * Ld;
    float c_height = (abs(height)-0.2 + 1)/2.0f;

    color = vec4(diffuse + texture(colormap, c_height).rgb, 1);
}
