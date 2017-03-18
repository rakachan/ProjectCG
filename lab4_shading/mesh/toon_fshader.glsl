#version 330
in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;
uniform float alpha;

out vec3 color;

uniform sampler1D tex1D;

void main() {
    color = vec3(0.0,0.0,0.0);

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 2.2: Toon shading.
    /// 1) compute ambient term.
    /// 2) compute diffuse term using the texture sampler tex.
    /// 3) compute specular term using the texture sampler tex.
    ///<<<<<<<<<< TODO <<<<<<<<<<<
    vec3 ambient = ka * La;

    float nl = texture(tex1D, max(dot(normal_mv, light_dir), 0)).x;
    vec3 diffuse = kd * nl * Ld;

    vec3 reflectn = normalize(reflect(-light_dir, normal_mv));
    float rv = texture(tex1D, pow(max(dot(reflectn, view_dir), 0), alpha)).x;
    vec3 specular = ks * rv * Ls;
    color =  ambient + diffuse + specular;
}
