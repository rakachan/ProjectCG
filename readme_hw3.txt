## Shading

### 3.1 Phong Shading

For the Phong Shading implementation we started by transforming the normals to camera space.
Compute the light direction and the view direction.
Then using our previous modifications we compute the color according to the formula: I = k_a*L_a + k_d*(n*l)*L_d + k_s*(r*v)^@ *L_s


### 3.2 Toon Shading

In this second part, your goal is to implement Toon Shading, which is also called cel
shading. This shading style mimics the rendering style of cartoons. The main idea is to
use only a discrete range of tones to shade the surface (see Figure 3).
Figure 3: Toon Shading
To create this shading style, you should start from Phong Shading and map n · l and r · v
to a discrete range stored in a 1D texture (see Figure 4).
Figure 4: A one dimentional texture with the toon shading mapping
This texture is already generated for you and stored in texture 1d id .
First, complete TODO 2.1 in mesh.h/bindShader() by binding texture 1d id (0.25
point). Then complete TODO 2.2 in toon fshader.glsl (0.75 point). You can use the
same vertex shader as in Phong Shading (phong vshader.glsl).




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

In this part of the homework your goal is to implement flat shading. Flat shading shades
each triangle based on the angle between the triangle normal and the light direction. This
is in contrast with smooth shading (phong, gouraud) see Figure 7. You should use use
dFdx and dFdy in the fragment shader to compute the triangle normal.
Figure 7: Flat (left) and Phong (right) Shading
First you should complete TODO 4.1 in flat vshader.glsl where you have to compute
the light direction light dir, compute the view direction view dir and pass
light dir, view dir and vpoint mv to the fragment shader. This first task is worth
0.5 point.
Next, you should complete TODO 4.2 in flat fshader.glsl in which you should
first compute the triangles normals using dFdx and dFdy and then add ambient, diffuse
and specular terms to shade the mesh. This second task is worth 1.5 point.


### 3.5 Spot Light Shading
In this last part of the homework your goal is to implement spot light shading. The main
idea is that the light rays are restricted by a cone of light (see Figure 8). Inside the cone
you should have
Ispot = I × SpotEffect
SpotEffect = (light dir · spot dir)
spot exp
Figure 8: Cone of light
Please note that the boundary of the cone should be strict and that the lighting inside the
cone should follow the above equation.
Complete TODO 5 in spot fshader.glsl. You can use the same vertex shader as in
phong vshader.glsl. You can use the mouse and left click to move the light. See
Figure 9. Note that I in the formula corresponds to Phong shading.
Figure 9: Spot Light Shading
