#pragma once
#include "icg_helper.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/rotate_vector.hpp>
#define PI 3.14159

using namespace glm;

class Trackball {
public:
    Trackball() : radius_(1.0f) {}

    // this function is called when the user presses the left mouse button down.
    // x, and y are in [-1, 1]. (-1, -1) is the bottom left corner while (1, 1)
    // is the top right corner.
    void BeingDrag(float x, float y) {
      anchor_pos_ = (vec3(x, y, 0.0f));
      //ProjectOntoSurface(anchor_pos_);
    }

    // this function is called while the user moves the curser around while the
    // left mouse button is still pressed.
    // x, and y are in [-1, 1]. (-1, -1) is the bottom left corner while (1, 1)
    // is the top right corner.
    // returns the rotation of the trackball in matrix form.
    vec3 Drag(float x, float y, vec3 dir, vec3 up = vec3(0, 0, 1)) {
      vec3 current_pos = (vec3(x, y, 0.0f));
      //ProjectOntoSurface(current_pos);
      //vec4 rotation = IDENTITY_MATRIX;
      //rotation = glm::rotate(rotation, 0.1f*acos(dot(normalize(anchor_pos_), normalize(current_pos))), cross(anchor_pos_, current_pos));
      vec3 d = dir;
      vec3 axis = cross(dir, up);
      d = glm::rotate(d,  2.0f * (current_pos.y - anchor_pos_.y), axis);
      d = glm::rotateZ(d, -3.14159f * (current_pos.x - anchor_pos_.x));
      if (abs(dot(normalize(d), up))>0.95) {
          d = glm::rotate(d, -2.0f *(current_pos.y - anchor_pos_.y), axis);
      }
      anchor_pos_ = current_pos;


      //rotation = IDENTITY_MATRIX;
      // TODO 3: Calculate the rotation given the projections of the anocher
      // point and the current position. The rotation axis is given by the cross
      // product of the two projected points, and the angle between them can be
      // used as the magnitude of the rotation.
      // you might want to scale the rotation magnitude by a scalar factor.
      // p.s. No need for using complicated quaternions as suggested inthe wiki
      // article.
      return d;
    }

private:
    // projects the point p (whose z coordiante is still empty/zero) onto the
    // trackball surface. If the position at the mouse cursor is outside the
    // trackball, use a hyberbolic sheet as explained in:
    // https://www.opengl.org/wiki/Object_Mouse_Trackball.
    // The trackball radius is given by 'radius_'.
    void ProjectOntoSurface(vec3& p) const {
      // TODO 2: Implement this function. Read above link for details.
      float norm2 = p.x*p.x+p.y*p.y;
      if (norm2<1/2.0f) {
          p.z = sqrt(1-norm2);
      }
      else {
          p.z = 1/(2*sqrt(norm2));
      }
    }

    float radius_;
    vec3 anchor_pos_;
    mat4 rotation_;
};
