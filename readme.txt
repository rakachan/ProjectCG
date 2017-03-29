### 3.1 Fast Gaussian Blur

### 3.2 Screen Space Reflections

This part is pretty straightforward. At the creation of the framebuffer, bind its texture to the floor.
When drawing, draw the cube in the framebuffer, then draw the floor (which will ten use the framebuffer's texture as its own).
In the shader, blend the original texture and the framebuffer texture.

### 3.3 Motion Blur