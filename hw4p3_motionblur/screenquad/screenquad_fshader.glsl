#version 330 core
in vec2 uv;
uniform sampler2D colorTex;
uniform sampler2D velocityTex;
out vec4 color;

void main() {
    int num_samples = 100;
    float scale = 4;
    vec2 velocity = texture(velocityTex, uv).xy * scale;
    velocity = vec2(0.0,0.0);
    vec4 Color = texture(colorTex, uv);

    for(int i = 1 ;i < num_samples ; ++i){
        Color += texture(colorTex, uv - velocity*i/float(num_samples));
    }

    color = Color/float(num_samples);
    //velocity = vec2(0.001,0);
    //velocity = vec2(0.001,0.001);
    /// TODO: use the velocity vector stored in velocityTex to compute the line integral
    /// TODO: use a constant number of samples for integral (what happens if you take too few?)
    /// HINT: you can scale the velocity vector to make the motion blur effect more prominent
    /// HINT: to debug integration don't use the velocityTex, simply assume velocity is constant, e.g. vec2(1,0)
    //color = texture(colorTex, uv);
}
