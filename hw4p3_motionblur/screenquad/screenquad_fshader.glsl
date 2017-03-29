#version 330 core
in vec2 uv;
uniform sampler2D colorTex;
uniform sampler2D velocityTex;
out vec4 color;

void main() {
    int num_samples = 27;
    float scale = 1;
    vec2 velocity = texture(velocityTex, uv).xy * scale/float(num_samples);
    vec3 Color = vec3(0,0,0);

    for(int i = 0 ;i < num_samples ; ++i){
        Color += clamp(texture(colorTex, uv - (velocity*i)).rgb, 0, 1);
    }

    Color /= float(num_samples);
    color = vec4(Color,1);
    /// TODO: use the velocity vector stored in velocityTex to compute the line integral
    /// TODO: use a constant number of samples for integral (what happens if you take too few?)
    /// HINT: you can scale the velocity vector to make the motion blur effect more prominent
    /// HINT: to debug integration don't use the velocityTex, simply assume velocity is constant, e.g. vec2(1,0)
    //color = texture(colorTex, uv);
}
