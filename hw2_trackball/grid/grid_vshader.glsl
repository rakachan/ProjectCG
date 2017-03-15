#version 330

in vec2 position;

out vec2 uv;

uniform mat4 MVP;
uniform float time;

void main() {
#if 0
    uv = (position + vec2(1.0, 1.0)) * 0.5;

    // convert the 2D position into 3D positions that all lay in a horizontal
    // plane.
    // TODO 6: animate the height of the grid points as a sine function of the
    // 'time' and the position ('uv') within the grid.
    float height = 0.1*sin(time+(uv.x+uv.y)*10);

#else
    uv = position;
    float radius = sqrt(uv.x*uv.x + uv.y*uv.y);
    float height = cos(time) * 0.5 * cos(14*radius - 4 * time) / (1 + 42*radius);
#endif
    vec3 pos_3d = vec3(position.x, height, -position.y);

    gl_Position = MVP * vec4(pos_3d, 1.0);
}
