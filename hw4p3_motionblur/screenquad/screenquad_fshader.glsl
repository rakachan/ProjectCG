#version 330 core
in vec2 uv;
uniform sampler2D colorTex;
uniform sampler2D velocityTex;
out vec4 color;

void main() {
    int num_samples = 70;
    /// TODO: use the velocity vector stored in velocityTex to compute the line integral
    vec2 velocity = texture(velocityTex, uv).xy * 10.0;
    //velocity = vec2(0,0);
    //I try debugging velocity here but it's hard to tell...
    //velocity = vec2(0.001,0.001);
    /**
    if(velocity.x !=0 || velocity.y != 0){
        color.y = 0.0;
    }
    **/
    vec2 TexCoord = uv;
    vec4 Color = texture(colorTex, TexCoord);

    for(int i = 1 ;i < num_samples ; ++i, TexCoord += velocity){
        Color += texture(colorTex, TexCoord);
    }

    color = Color/float(num_samples);
    /// TODO: use a constant number of samples for integral (what happens if you take too few?)
    /// HINT: you can scale the velocity vector to make the motion blur effect more prominent
    /// HINT: to debug integration don't use the velocityTex, simply assume velocity is constant, e.g. vec2(1,0)
    //color = texture(colorTex, uv);
}
