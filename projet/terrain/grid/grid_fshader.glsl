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

out vec3 color;

uniform sampler2D tex;
uniform sampler2D tex_snow;
uniform sampler2D tex_rock;
uniform sampler2D tex_sand;
uniform sampler2D tex_grass;


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

    float height = (texture(tex, uv).r + 1)/2.0f;


    float blend_region = 0.01;
    float blend_length = 2*blend_region;
    float length_region = 0.2;
    /*
    float a0 = 0;//1.0 - abs(height - 0.5)/blend_region;
    float a1 = 1.0 - abs(height - 0.6)/blend_region;
    float a2 = 1.0 - abs(height - 0.8)/blend_region;
    float a3 = 1.0 - abs(height - 0.4)/(0.5*blend_region);
    */

    float a0 = 0.0;
    float a1 = 0.0;
    float a2 = 0.0;
    float a3 = 0.0;

    float snow_height = 0.75;
    float rock_height = 0.6;
    float grass_height = 0.41;


    if (height > snow_height){//snow
        a2 = 1.0;
        if(height < snow_height + blend_region) {
            a2 = 1.0 - abs(height - (snow_height + blend_region))/blend_length;
            a1 = 1.0 - abs(height - (snow_height - blend_region))/blend_length;
        }

    }  else if (height > rock_height) {//rock
        a1 = 1.0;
        if(height < rock_height + 10*blend_region) {
            a1 = 1.0 - abs(height - (rock_height + 10*blend_region))/(blend_length*10);
            a0 = 1.0 - abs(height - (rock_height - 10*blend_region))/(10*blend_length);
        } else if (height > snow_height - blend_region) {
            a2 = 1.0 - abs(height - (snow_height + blend_region))/blend_length;
            a1 = 1.0 - abs(height - (snow_height - blend_region))/blend_length;
        }

    } else if (height > grass_height) { //grass
        a0 = 1.0;
        if(height < grass_height + blend_region) {
            a0 = 1.0 - abs(height - (grass_height + blend_region))/blend_length;
            a3 = 1.0 - abs(height - (grass_height - blend_region))/blend_length;
        } else if (height > rock_height - 10*blend_region) {
            a1 = 1.0 - abs(height - (rock_height + 10*blend_region))/(10*blend_length);
            a0 = 1.0 - abs(height - (rock_height - 10*blend_region))/(10*blend_length);
        }

    } else {//sand
        a3 = 1.0;
        if (height > grass_height - blend_region) {
            a3 = 1.0 - abs(height - (grass_height - blend_region))/blend_length;
            a0 = 1.0 - abs(height - (grass_height + blend_region))/blend_length;
        }
    }

    a0 = a0 < 0 ? 0 : a0;
    a1 = a1 < 0 ? 0 : a1;
    a2 = a2 < 0 ? 0 : a2;
    a3 = a3 < 0 ? 0 : a3;


    vec3 snow = texture(tex_snow, 30*uv).rgb;
    vec3 rock = texture(tex_rock, 10*uv).rgb;
    vec3 col = texture(tex_grass, 10*uv).rgb;
    vec3 sand = texture(tex_sand, 10*uv).rgb;

    color = diffuse + a0 * col + a1 * rock + a2 * snow + a3 * sand;
}
