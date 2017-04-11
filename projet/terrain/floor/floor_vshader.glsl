#version 330 core
uniform mat4 MVP;
uniform float time;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
uniform vec3 light_pos;

out vec3 light_dir;
out vec3 view_dir;
out vec4 vpoint_mv;

in vec2 position;

out vec2 uv;
out float height_modif;

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.5;
    height_modif = 0.02*(sin(time)*sin(time+uv.x*uv.x*30)*sin(time+uv.y*uv.y*30))-0.2;
    mat4 MV = V*M;
    vec3 pos_3d = vec3(position.x, position.y, height_modif);
    vpoint_mv = MV * vec4(pos_3d, 1.0);
    gl_Position = P*vpoint_mv;
    light_dir=normalize(light_pos - vpoint_mv.xyz);
    view_dir = normalize(-vpoint_mv.xyz);
}
