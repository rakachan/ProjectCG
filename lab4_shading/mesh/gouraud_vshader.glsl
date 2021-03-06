#version 330

in vec3 vpoint;
in vec3 vnormal;

uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;
uniform float alpha;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light_pos;

out vec3 vcolor;

void main() {
    mat4 MV = view * model;
    vec4 vpoint_mv = MV * vec4(vpoint, 1.0);
    gl_Position = projection * vpoint_mv;
    //>>>>>>>>>> TODO >>>>>>>>>>>
    // TODO 0.1: Gouraud shading.
    // 1) compute the normal using the model_view matrix.
    // 2) compute the light direction light_dir.
    // 3) compute the view direction view_dir.
    // 4) compute per vertex color
    //<<<<<<<<<< TODO <<<<<<<<<<<
    vec3 normal_mv = normalize(mat3(inverse(transpose(MV))) * vnormal);

    vec3 light_dir = normalize(light_pos - vpoint_mv.xyz);
    vec3 view_dir = normalize(-vpoint_mv.xyz);

    vec3 ambient = ka * La;

    vec3 nl = vec3(clamp(dot(normal_mv, light_dir), 0, 1));
    vec3 diffuse = kd * nl * Ld;

    vec3 reflectn = normalize(reflect(-light_dir, normal_mv));
    vec3 rv = vec3(pow(clamp(dot(reflectn, view_dir), 0, 1), alpha));
    vec3 specular = ks * rv * Ls;
    vcolor =  ambient + diffuse + specular;
}
