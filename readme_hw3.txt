## Shading

### 3.1 Phong Shading

For the Phong Shading implementation we started by transforming the normals to camera space.
Compute the light direction and the view direction.
Then using our previous modifications we compute the color according to the formula: 
I = k_a*L_a + k_d*(n*l)*L_d + k_s*(r*v)^@ *L_s


### 3.2 Toon Shading

For this second par, we implemented Toon Shading by storing a discrete range of colors in
a 1D texture (allready given to us) and mapping n*l and r*v to it. We first bound the shader
to the texture, then compute ambient term, diffuse term and specular term using the texture
sampler tex to define the color of the shader.


### 3.3 artistic Shading

In this part of the homework your goal is to use a 2D texture to shade the object in an
artistic fashion (see Figure 5). As in Toon shading, artistic shading uses a texture with the
difference that now this texture is in 2D (see Figure 6).
Figure 5: Artistic Shading
Read http://maverick.inria.fr/Publications/2006/BTM06a/x-toon.pdf for more informations
on artistic shading.
First, complete TODO 3.1 in mesh.h/bindShader() by binding texture 2d id (0.25
point). Then complete TODO 3.2 in art fshader.glsl (0.75 point). You can use the
same vertex shader as in Phong Shading (phong vshader.glsl).
Figure 6: A 2D texture with the artistic shading mapping


### 3.4 Flat Shading

The vectors are calculated in the exact same way as in the practical.
To compute the normal vectors using dFdx and dFdy, one must know that these 
functions return the variation of the given parameterbetween two adjacent pixels
(hence the x and y). Thus, these funtions are useful to compute the gradient in the
two direction. The normal vector is derived by taking the cross product.



### 3.5 Spot Light Shading

Here we have to compute the spot effect with the given formula. Then we compute the cos of the
angle between light_dir and spot_dir (the two vector are normalized) if the angle is upper than
the spot_cos_cutoff (i.e. the angle is smaller) we apply the formula on the other case we just 
let the ambiant light. 

