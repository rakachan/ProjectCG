### 3.1 Fast Gaussian Blur

### 3.2 Screen Space Reflections

This part is pretty straightforward. At the creation of the framebuffer, bind its texture to the floor.
When drawing, draw the cube in the framebuffer, then draw the floor (which will ten use the framebuffer's texture as its own).
In the shader, blend the original texture and the framebuffer texture.

### 3.3 Motion Blur
First, we compute the screen space motion vector in the quad_fshader,
by simple vectorial substraction between currentPosand previousPos.
Then it's all sent to the screenquad_fshader where it's used to
determinethe intensity and the direction of the blur.
Finally we compute the resulting color for a given (position,motionVector,texture)
by averaging the image, and slight offset displacement, along the motionVector.
