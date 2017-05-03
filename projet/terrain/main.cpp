// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#include "icg_helper.h"

#include "framebuffer.h"
#include "trackball.h"

#include "floor/floor.h"
#include "cube/cube.h"
#include "grid/grid.h"
#include "screenquad/screenquad.h"
#include "quad/quad.h"


using namespace glm;

int window_width = 1280;
int window_height = 720;

double y_anch;

float time;

mat4 projection_matrix;
mat4 view_matrix;
mat4 refl_matrix;
//mat4 view_proj;
//mat4 view_refl;
mat4 trackball_matrix;
mat4 old_trackball_matrix;

FrameBuffer framebuffer;
FrameBuffer reflexion;
Floor water;
Cube cube;
Grid grid;
Quad quad;
ScreenQuad screenquad;

Trackball trackball;

using namespace glm;

void Init(GLFWwindow* window) {
    glClearColor(1.0, 1.0, 1.0 /*white*/, 1.0 /*solid*/);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    trackball_matrix = IDENTITY_MATRIX;

    glfwGetFramebufferSize(window, &window_width, &window_height);

    GLuint framebuffer_texture_id;
    framebuffer_texture_id = framebuffer.Init(window_width, window_height);

    GLuint reflexion_texture_id;
    reflexion_texture_id = reflexion.Init(window_width, window_height, false, GL_RGB);

    screenquad.Init(window_width, window_height, framebuffer_texture_id);
    grid.Init(framebuffer_texture_id);
    quad.Init();
    water.Init(framebuffer_texture_id, reflexion_texture_id);

    vec3 cam_pos(2.0f, 2.0f, 2.0f);
    vec3 cam_look(0.0f, 0.0f, 0.0f);
    vec3 cam_up(0.0f, 0.0f, 1.0f);

    vec3 cam_refl(2.0f, 2.0f, -2.0f);
    vec3 up_refl(0.0f, 0.0f, 1.0f);

    view_matrix = lookAt(cam_pos, cam_look, cam_up);
    refl_matrix = lookAt(cam_refl, cam_look, up_refl);
    float ratio = window_width / (float) window_height;
    projection_matrix = perspective(45.0f, ratio, 0.1f, 10.0f);
    //view_proj = projection_matrix*view_matrix;
    //view_refl = projection_matrix*refl_matrix;
}

void Display() {
    //glViewport(0,0,window_width,window_height);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    time = glfwGetTime();
    framebuffer.Clear();
    framebuffer.Bind();
        quad.Draw(IDENTITY_MATRIX, IDENTITY_MATRIX, IDENTITY_MATRIX);
    framebuffer.Unbind();
    reflexion.Clear();
    reflexion.Bind();
        /*glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);*/
        grid.Draw(trackball_matrix, view_matrix, projection_matrix, time, 0, 1);
        //glDisable(GL_CULL_FACE);
    reflexion.Unbind();
    glViewport(0, 0, window_width, window_height);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    grid.Draw(trackball_matrix, view_matrix, projection_matrix, time);
    //grid.Draw(trackball_matrix, view_matrix, projection_matrix, time, 0, 1);
    water.Draw(trackball_matrix, view_matrix, projection_matrix, time);

}

// Gets called when the windows/framebuffer is resized.
void resize_callback(GLFWwindow* window, int width, int height) {
    glfwGetFramebufferSize(window, &window_width, &window_height);
    float ratio = window_width / (float) window_height;
    projection_matrix = perspective(45.0f, ratio, 0.1f, 10.0f);
    glViewport(0, 0, window_width, window_height);
    framebuffer.Cleanup();
    framebuffer.Init(window_width, window_height);
    reflexion.Cleanup();
    reflexion.Init(window_width, window_height, false, GL_RGB);
    screenquad.UpdateSize(window_width, window_height);
}

void ErrorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_ESCAPE :
                glfwSetWindowShouldClose(window, GL_TRUE);
            break;
            case GLFW_KEY_Q :
                quad.changeAmp(-0.05);
            break;
            case GLFW_KEY_W :
                quad.changeAmp(0.05);
            break;
            case GLFW_KEY_A :
                quad.changePers(-0.05);
            break;
            case GLFW_KEY_S :
                quad.changePers(0.05);
            break;
            case GLFW_KEY_Z : //maps to Y key
                quad.changeFreq(-0.3);
            break;
            case GLFW_KEY_X :
                quad.changeFreq(0.3);
            break;
            default:break;
        }
    }
}

// transforms glfw screen coordinates into normalized OpenGL coordinates.
vec2 TransformScreenCoords(GLFWwindow* window, int x, int y) {
    // the framebuffer and the window doesn't necessarily have the same size
    // i.e. hidpi screens. so we need to get the correct one
    int width;
    int height;
    glfwGetWindowSize(window, &width, &height);
    return vec2(2.0f * (float)x / width - 1.0f,
                1.0f - 2.0f * (float)y / height);
}

void MouseButton(GLFWwindow* window, int button, int action, int mod) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x_i, y_i;
        glfwGetCursorPos(window, &x_i, &y_i);
        vec2 p = TransformScreenCoords(window, x_i, y_i);
        trackball.BeingDrag(p.x*2-1, p.y*2-1);
        old_trackball_matrix = trackball_matrix;
        // Store the current state of the model matrix.
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        double x_i, y_i;
        glfwGetCursorPos(window, &x_i, &y_i);
        y_anch = y_i;
    }
}

void MousePos(GLFWwindow* window, double x, double y) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        vec2 p = TransformScreenCoords(window, x, y);
        // TODO 3: Calculate 'trackball_matrix' given the return value of
        // trackball.Drag(...) and the value stored in 'old_trackball_matrix'.
        // See also the mouse_button(...) function.
        // trackball_matrix = ...
        trackball_matrix = trackball.Drag(p.x*2-1, p.y*2-1) * old_trackball_matrix;
    }

    // zoom
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        // TODO 4: Implement zooming. When the right mouse button is pressed,
        // moving the mouse cursor up and down (along the screen's y axis)
        // should zoom out and it. For that you have to update the current
        // 'view_matrix' with a translation along the z axis.
        float c = (y-y_anch)/50.0f;
        view_matrix = translate(view_matrix, vec3(c, c, 2.0f*c));
        refl_matrix = translate(refl_matrix, vec3(c, c, 2.0f*c));
        y_anch = y;
    }
}


int main(int argc, char *argv[]) {
    /// GLFW Initialization
    if( !glfwInit() ){
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(ErrorCallback);



    /// Hint GLFW that we would like an OpenGL 3 context (at least)
    /// http://www.glfw.org/faq.html#how-do-i-create-an-opengl-30-context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /// Attempt to open the window: fails if required version unavailable
    /// @note some Intel GPUs do not support OpenGL 3.2
    /// @note update the driver of your graphic card
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "mirror_floor", NULL, NULL);
    if( !window ){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /// Link the current context to the window we created
    glfwMakeContextCurrent(window);

    /// Set the callback for escape key
    glfwSetKeyCallback(window, KeyCallback);

    /// set the mouse press and position callback
    glfwSetMouseButtonCallback(window, MouseButton);
    glfwSetCursorPosCallback(window, MousePos);

    glfwSetFramebufferSizeCallback(window, resize_callback);

    /// GLEW Initialization (must have a context)
    glewExperimental = GL_TRUE; ///<
    if( glewInit() != GLEW_NO_ERROR ){
        fprintf( stderr, "Failed to initialize GLEW\n");
        exit(EXIT_FAILURE);
    }

    std::cout << "OpenGL" << glGetString(GL_VERSION) << std::endl;
    Init(window);
    KeyCallback(window, GLFW_KEY_KP_1, 0, 0, 0);

    /// Render loop & keyboard input
    while(!glfwWindowShouldClose(window)){
        Display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    grid.Cleanup();
    screenquad.Cleanup();
    quad.Cleanup();
    water.Cleanup();

    /// Close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    exit( EXIT_SUCCESS );
}

