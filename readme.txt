### 3.1 Fast Gaussian Blur
We first create the 1D kernel and pass it as uniform. Then in framebuffer.h, create a second texture.
When drawing, first draw the cube and quad in the framebuffer, then swap to the second texture in the same COLOR_ATTACHMENT, then draw the screenquad by telling it to blur vertically and taking the first texture as input.
Finally draw again the screen quad, but outside the framebuffer and telling it to blur horizontally and taking the second texture as input.

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
