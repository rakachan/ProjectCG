#version 330
in vec2 uv;

out vec3 color;

uniform sampler1D colormap;

void main() {
    bool BW = false;
    float pi = 3.14;
    float coef = 6;
    float sincos = ((sin(2*coef*pi*uv.x)*sin(2*coef*pi*uv.y)));
    if (BW) {
        if (sincos>0) {
            color = vec3(0, 0, 0);
        }
        else {
            color = vec3(1, 1, 1);
        }
    }
    else {
        color = texture(colormap, (sincos+1)/2).rgb;
    }
}
