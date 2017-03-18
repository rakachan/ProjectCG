#version 330
in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;

out vec3 color;

uniform float alpha;
uniform sampler2D tex2D;

void main() {
    color = vec3(0.0, 0.0, 0.0);

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
    // TODO 3.2: Artistic shading.
    // 1) compute the output color by doing a look-up in the texture using the
    //    texture sampler tex.
    //<<<<<<<<<< TODO <<<<<<<<<<<

    vec3 ambient = ka * La;
    vec3 reflectn = normalize(reflect(-light_dir, normal_mv));

    vec3 art = texture(tex2D, vec2(max(dot(normal_mv, light_dir), 0), pow(max(dot(reflectn, view_dir), 0), alpha))).rgb;

    vec3 nl = art;
    vec3 diffuse = kd * nl * Ld;

    vec3 rv =  art;
    vec3 specular = ks * rv * Ls;
    color =  art;
}
