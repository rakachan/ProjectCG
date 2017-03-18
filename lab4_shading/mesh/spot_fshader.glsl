#version 330

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

out vec3 color;

uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;
uniform float alpha;
uniform vec3 spot_dir;


const float spot_cos_cutoff = 0.985; // cos 10 degrees
const float spot_exp = 150;

void main() {    
    const vec3 COLORS[6] = vec3[](
        vec3(1.0,0.0,0.0),
        vec3(0.0,1.0,0.0),
        vec3(0.0,0.0,1.0),
        vec3(1.0,1.0,0.0),
        vec3(0.0,1.0,1.0),
        vec3(1.0,0.0,1.0));
    int index = int( mod(gl_PrimitiveID,6) );
    color = COLORS[index];

    //>>>>>>>>>> TODO >>>>>>>>>>>
    // TODO 5: Spot light.
    // Complete the shader to obtain spot light effect
    //<<<<<<<<<< TODO <<<<<<<<<<<

    float angle = dot(light_dir, spot_dir);
    vec3 spotEffect = vec3(1.0);
    if(angle < spot_cos_cutoff) {
        spotEffect = vec3(1.0)*pow(angle, spot_exp);
    }

    vec3 ambient = ka * La;

    vec3 nl = vec3(clamp(dot(normal_mv, light_dir), 0, 1));
    vec3 diffuse = kd * nl * Ld;

    vec3 reflectn = normalize(reflect(-light_dir, normal_mv));
    vec3 rv = vec3(pow(clamp(dot(reflectn, view_dir), 0, 1), alpha));
    vec3 specular = ks * rv * Ls;
    //color =  cross(ambient + diffuse + specular, spotEffect);

}
