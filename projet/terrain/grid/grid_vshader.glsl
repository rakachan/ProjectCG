#version 330

in vec2 position;

out vec2 uv;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
uniform vec3 light_pos;

uniform mat4 MVP;
uniform sampler2D heightmap;
uniform sampler2D tex;

out vec3 light_dir;
out vec3 view_dir;
out vec4 vpoint_mv;

void main() {
    mat4 MV = V*M;
    uv = (position + vec2(1.0, 1.0)) * 0.5;
    float height = texture(tex, uv).r;
    vec3 pos_3d = vec3(position.x, position.y, height);
    vpoint_mv = MV * vec4(pos_3d, 1.0);
    gl_Position = P*vpoint_mv;
    light_dir=normalize(light_pos - vpoint_mv.xyz);
    view_dir = normalize(-vpoint_mv.xyz);
}
