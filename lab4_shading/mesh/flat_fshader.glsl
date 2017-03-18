#version 330

in vec4 vpoint_mv;
in vec3 light_dir, view_dir;

out vec3 color;

uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;
uniform float alpha;


void main() {
    color = vec3(0.0,0.0,0.0);
    const vec3 COLORS[6] = vec3[](
        vec3(1.0,0.0,0.0),
        vec3(0.0,1.0,0.0),
        vec3(0.0,0.0,1.0),
        vec3(1.0,1.0,0.0),
        vec3(0.0,1.0,1.0),
        vec3(1.0,0.0,1.0));
    int index = int( mod(gl_PrimitiveID,6) );
    color = COLORS[index];

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 4.2: Flat shading.
    /// 1) compute triangle normal using dFdx and dFdy
    /// 1) compute ambient term.
    /// 2) compute diffuse term.
    /// 3) compute specular term.
    ///<<<<<<<<<< TODO <<<<<<<<<<<
    vec3 normal_tr = normalize(cross(dFdx(vpoint_mv.xyz), dFdy(vpoint_mv.xyz)));

    vec3 ambient = ka * La;

    vec3 nl = vec3(clamp(dot(normal_tr, light_dir), 0, 1));
    vec3 diffuse = kd * nl * Ld;

    vec3 reflectn = normalize(reflect(-light_dir, normal_tr));
    vec3 rv = vec3(pow(clamp(dot(reflectn, view_dir), 0, 1), alpha));
    vec3 specular = ks * rv * Ls;
    color =  ambient + diffuse + specular;
}
