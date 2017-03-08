1.1 Triangle Spirals

In this problem, we were given the code to draw a simple triangle,
with the possibility of passing a 4x4 matrix as an argument in order to
modify the triangle.
We had two spirals to draw.
For the first one, all we had to do was to scale down the triangle,
translate it along the x (or the y) axis, then rotate. Note that at
each iteration, the angle of rotation, the distance of translation and
the scaling increases, to finally get the wanted figure.
For the second one, the Fermat Spiral, all we had to do is search for the
values for the translation and the rotation.

In order to either draw the Fermat or the regular spiral in the program,
in the "main.cpp" file, you simply need to change the value of the
FERMAT macro to "true" or "false".

1.2 Checkerboard

Here, we had to remember the checkerboard function given in class,
which was f(x) = sin(2x)sin(2y), and give to the texture function
both the colormap and this implicit function (after normalizing it
to fit in [0, 1].
Another interesting construction, rather than using the texture
function, is to do what was explained in class and simply check
the positivity of the implicit function. This gives us a pretty good
Black and White checkerboard. To see what I mean, set the BW variable
in the file "quad_fshader.glsl" to true.

1.3 Solar System

For the sun, we just need to scale it down, make it rotate and then translate it.
For the earth, we need to scale it scale it down, make it rotate on itself and
then applying the revolution matrix.
For the moon, we need to scale it down, translate it and applying the rotation
matrix. This will make it revolve around the earth. At the end, we apply the
revolution matrix.

The revolution matrix E is a translation matrix which varies with the angle
followed by a rotation matrix.

The speed is adapted such that time_s represent a day, the moon revolves
around the earth in 27.3 days, the earth and the moon revolve around the
sun in 356 days and the sun rotates on itself in 38 days.
