#version 330 core
uniform float time;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
uniform vec3 light_pos;
uniform vec3 cam_pos;

out vec4 vpoint_mv;
out mat4 MV;

in vec2 position;

out vec2 uv;
out float height_modif;
out vec3 pos_3d;

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.5;
    height_modif = 0;//0.02*(sin(time)*sin(time+uv.x*20)*sin(time+uv.y*20));
    MV = V*M;
    pos_3d = vec3(position.x, position.y, height_modif);
    vpoint_mv = MV * vec4(pos_3d, 1.0);
    gl_Position = P*vpoint_mv;
    //light_dir= normalize(pos_3d - light_pos);
    //view_dir = normalize(pos_3d - cam_pos);
    //normal_mv = normalize(mat3(inverse(transpose(MV))) * texture(normalmap, uv).rgb);
    //normal_mv = normalize(texture(normalmap, 10.0f*uv+(0.01f*time)).rgb);
}
