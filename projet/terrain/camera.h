#pragma once
#include "icg_helper.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include "bezier.h"
#define PI 3.14159

using namespace glm;

enum Mode {FREE, FPS, BEZIER};
enum Key {UP, LEFT, DOWN, RIGHT, O_KEY, P_KEY};

class Camera {
private:
    vec3 cam_pos;
    vec3 cam_look;
    vec3 cam_dir;
    vec3 cam_up;

    vec3 anchor_pos_;
    vec3 mov_dir;

    mat4 view_matrix;

    float y_anch;
    Mode mode;
    GLuint height_id;
    int width;
    int height;
    float h;

    Bezier bezier;
    float t;
    float bezier_time = 300.0f;

public:
    void Init(vec3 pos, vec3 look, vec3 up, int width, int height, GLuint height_texture_id) {

        cam_pos = pos;
        cam_look = look;
        cam_up = up;
        cam_dir = cam_look-cam_pos;
        mode = FREE;
        height_id = height_texture_id;
        this->height = height;
        this->width = width;
        setView();
        h = 0.0;

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

    void computeHeight() {

        if (cam_pos.x > 1 || cam_pos.x < -1 || cam_pos.y > 1 || cam_pos.y < -1) {
            h = -0.3;
        }
        vec2 coord = mapPixel(cam_pos);
        glReadPixels(coord.x, coord.y, 1, 1, GL_RED, GL_FLOAT, &h);
        glGetError();

    }

    void Drag(float x, float y) {
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

    void zoom(float y) {
        if (mode!=BEZIER) {
            float c = (y-y_anch)/50.0f;
            cam_pos = cam_pos+vec3(c, c, c);
            cam_dir = cam_look - cam_pos;
            setView();
            y_anch = y;
        }
    }

    void Draw() {
        switch(mode) {
        case FPS:
            cam_pos=clamp(cam_pos+0.01f*mov_dir, vec3(-0.99f, -0.99f, -1.0f), vec3(0.99f, 0.99f, 1.0f));
            cam_look = cam_pos + cam_dir;
            //cam_look=cam_look+0.01f*mov_dir;
            mov_dir=0.9f*mov_dir;
            setCamToHeight(cam_pos.x, cam_pos.y);
            setView();
            break;
        case FREE:
            cam_pos=cam_pos+0.01f*mov_dir;
            cam_look=cam_look+0.01f*mov_dir;
            mov_dir=0.9f*mov_dir;
            setView();
        break;
        case BEZIER:
            useBezier(t/bezier_time);
            setView();
            t+=1;
            if ((t>=bezier_time)) {
                t=0.0f;
            }
        break;
        default: break;
        }

    }

    vec2 mapPixel(vec3 position) {

        if (position.x > 1 || position.x < -1 || position.y > 1 || position.y < -1) {
            return vec2(0);
        }

        vec3 tmp = position + vec3(1, 1, 0);
        return vec2(tmp.x/2.0* width, tmp.y/2.0 * height);
    }

    void setMode(Mode m) {
        mode = m;
        if (mode == FPS) {
            setCamToHeight(cam_pos.x, cam_pos.y);
        }
    }

    void setCamToHeight(float x, float y) {
        cam_pos = vec3(x, y, h + 0.3);
    }

    void setMov(Key key) {
        if (mode==FREE || mode==FPS) {
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

    void modifyBezierTime(Key key) {
        float old_bezier = bezier_time;
        switch(key) {
        case O_KEY:
            bezier_time += 10.0f;
        break;
        case P_KEY:
            bezier_time -= 10.0f;
        break;
        default: break;
        }
        if (bezier_time<100) {
            bezier_time = 100;
        }
        if (bezier_time>500) {
            bezier_time = 500;
        }
        t = t/(old_bezier/bezier_time);
    }

    void setBezier(vector<vec3> points) {
        bezier.Init(points);
        t = 0;
        mode = BEZIER;
    }

    void useBezier(float t) {
        cam_pos = bezier.getPos(t);
        cam_dir = cam_look - cam_pos;
    }

    mat4 getView() {
        return view_matrix;
    }

    vec3 getPos() {
        return cam_pos;
    }

    vec3 getLook() {
        return cam_look;
    }

    vec3 getUp() {
        return cam_up;
    }
};

