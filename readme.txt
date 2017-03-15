1 Perspective Projection

In this problem, we just had to enter manually the values given in the
class' slides, ch. 3b, page 36.


2 Trackball

Once again, for TODO2, we merely need to implement the formulas given
at https://www.khronos.org/opengl/wiki/Object_Mouse_Trackball

Regarding TODO3, in order to get the angle between the anchor and the 
current position, one only needs to calculate the dot product of them 
(after normalization, of course). Similarly, the angle of rotation
corresponds to the cross product of those said vectors. Applying the 
rotation is straightforward.

TODO4 was the trickiest. In order to handle the zooming correctly, one
needs to have an anchor that updates on every movement. In order to do
this, we needed to initialize the anchor before even moving, which is done
upon clicking the right_mouse button. Once that is done, upon moving the 
mouse, we can compare the new position to the anchor, translate our camera,
then update the anchor with current position.


3 Triangle Grid And Animation (2 points)

A for-loop is mandatory here. The only real issue is to take care of the
indices of the vertices. A pen and paper drawing helps in this task.
mapping the grid position to a moving sine was straight-forward. Let 
the sine have phase depending on x and y (here, the function was (x+y)/10),
and add the time inside. scale the sine down for visibility.