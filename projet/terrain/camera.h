#pragma once
#include "icg_helper.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/rotate_vector.hpp>
#define PI 3.14159

enum Mode {FREE, FPS, BEZIER};
enum Key {UP, LEFT, DOWN, RIGHT};

class Camera {
public:
    vec3 cam_pos;
    vec3 cam_look;
    vec3 cam_dir;
    vec3 cam_up;

    vec3 anchor_pos_;
    vec3 mov_dir;

    mat4 view_matrix;

    float y_anch;
    Mode mode;

    void Init(vec3 pos, vec3 look, vec3 up) {
        cam_pos = pos;
        cam_look = look;
        cam_up = up;
        cam_dir = cam_look-cam_pos;
        mode = FREE;
        setView();
    }

    void setView() {
        view_matrix = lookAt(cam_pos, cam_look, cam_up);
    }

    void setAnchorPos(float x, float y) {
        anchor_pos_ = vec3(x, y, 0.0f);
    }

    void setYAnch(float y) {
        y_anch = y;
    }

    void Drag(float x, float y) {
      if (mode==FREE) {
          vec3 current_pos = (vec3(x, y, 0.0f));
          vec3 d = cam_dir;
          vec3 axis = cross(cam_dir, cam_up);
          d = glm::rotate(d,  2.0f * (current_pos.y - anchor_pos_.y), axis);
          d = glm::rotateZ(d, -3.14159f * (current_pos.x - anchor_pos_.x));
          if (abs(dot(normalize(d), cam_up))>0.95) {
              d = glm::rotate(d, -2.0f *(current_pos.y - anchor_pos_.y), axis);
          }
          anchor_pos_ = current_pos;
          cam_dir = d;
          cam_look = cam_pos + cam_dir;
          setView();
      }
    }

    void zoom(float y) {
        float c = (y-y_anch)/50.0f;
        cam_pos = cam_pos+vec3(c, c, c);
        cam_dir = cam_look - cam_pos;
        setView();
        y_anch = y;
    }

    void Draw() {
        cam_pos=cam_pos+0.01f*mov_dir;
        cam_look=cam_look+0.01f*mov_dir;
        mov_dir=0.9f*mov_dir;
        setView();
    }

    void setMov(Key key) {
        if (mode==FREE) {
            switch(key) {
                case UP:
                    mov_dir = normalize(cam_dir);
                break;
                case DOWN:
                    mov_dir = -normalize(cam_dir);
                break;
                case LEFT:
                    mov_dir = -normalize(cross(cam_dir, cam_up));
                break;
                case RIGHT:
                    mov_dir = normalize(cross(cam_dir, cam_up));
                break;
                default: break;
            }
        }
    }
};

