#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex;
uniform float persistence_key;
uniform float amplitude_key;
uniform float frequency_key;

vec4 mod289(vec4 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x)
{
  return mod289(((x*34.0)+1.0)*x);
}

vec2 fade(vec2 x){
    return 6*x*x*x*x*x -15*x*x*x*x + 10*x*x*x;
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float perlin(vec2 p, float grid_dim) {

    vec4 Pi = floor(vec4(uv.x, uv.y, uv.x, uv.y) * grid_dim) + vec4(0.0, 0.0, 1.0, 1.0) ;
    vec4 Pf = fract(vec4(uv.x, uv.y, uv.x, uv.y) * grid_dim) - vec4(0.0, 0.0, 1.0, 1.0) ;
    Pi = mod289(Pi); // To avoid truncation effects in permutation
    vec4 ix = Pi.xzxz;
    vec4 iy = Pi.yyww;
    vec4 fx = Pf.xzxz;
    vec4 fy = Pf.yyww;

    vec4 i = permute(permute(ix) + iy);

    vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
    vec4 gy = abs(gx) - 0.5 ;
    vec4 tx = floor(gx + 0.5);
    gx = gx - tx;

    vec2 g00 = vec2(gx.x,gy.x);
    vec2 g10 = vec2(gx.y,gy.y);
    vec2 g01 = vec2(gx.z,gy.z);
    vec2 g11 = vec2(gx.w,gy.w);

    vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
    g00 *= norm.x;
    g01 *= norm.y;
    g10 *= norm.z;
    g11 *= norm.w;

    float n00 = dot(g00, vec2(fx.x, fy.x));
    float n10 = dot(g10, vec2(fx.y, fy.y));
    float n01 = dot(g01, vec2(fx.z, fy.z));
    float n11 = dot(g11, vec2(fx.w, fy.w));

    vec2 fade_xy = fade(Pf.xy);
    vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
    float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
    return 2.4 * n_xy;
}

float fBm(vec2 p, int octaves) {
    float total = 0.0;
    float frequency = frequency_key;
    float amplitude = 0.5;
    float maxValue = 0.0;  // Used for normalizing result to 0.0 - 1.0
    for(int i=0;i<octaves;i++) {
        total += perlin(p, frequency) * amplitude * (2 / frequency_key) ;

        maxValue += amplitude;

        amplitude *= persistence_key;
        frequency *= amplitude_key;
    }

    return total/maxValue;
}

void main() {

    color = vec3(fBm(uv, 10));
}
