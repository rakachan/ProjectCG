// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#include "icg_helper.h"
#include "glm/gtc/matrix_transform.hpp"

#include "framebuffer.h"

#include "cube/cube.h"
#include "quad/quad.h"
#include "screenquad/screenquad.h"

Cube cube;
Quad quad;

int window_width = 800;
int window_height = 600;

// TODO: declare Framebuffer + ScreenQuad (see slides)

FrameBuffer framebuffer;
ScreenQuad screenquad;

using namespace glm;

mat4 projection_matrix;
mat4 view_matrix;
mat4 cube_model_matrix;

void Init(GLFWwindow* window) {
    glClearColor(1.0, 1.0, 1.0 /*white*/, 1.0 /*solid*/);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    cube.Init();
    quad.Init();

    // setup view and projection matrices
    vec3 cam_pos(2.0f, 2.0f, 2.0f);
    vec3 cam_look(0.0f, 0.0f, 0.0f);
    vec3 cam_up(0.0f, 0.0f, 1.0f);
    view_matrix = lookAt(cam_pos, cam_look, cam_up);

    float ratio = window_width / (float) window_height;
    projection_matrix = perspective(45.0f, ratio, 0.1f, 10.0f);

    // create the model matrix (remember OpenGL is right handed)
    // accumulated transformation
    cube_model_matrix = scale(IDENTITY_MATRIX, vec3(0.5));
    cube_model_matrix = translate(cube_model_matrix, vec3(0.0, 0.0, 0.6));

    // TODO: initialize framebuffer (see slides)
    // TODO: initialize fullscreen quad (see slides)
    glfwGetFramebufferSize(window, &window_width, &window_height);
    framebuffer.Cleanup();
    GLuint framebuffer_texture_id;
    GLuint framebuffer_tmp_texture_id;
    std::tie(framebuffer_texture_id, framebuffer_tmp_texture_id) =
            framebuffer.Init(window_width, window_height);
    screenquad.Init(window_width, window_height,
                    framebuffer_texture_id, framebuffer_tmp_texture_id);
    framebuffer.Cleanup();
    framebuffer.Init(window_width, window_height);
}

void Display() {
    // TODO: wrap these calls so they render to a texture (see slides)
    framebuffer.Clear();
    framebuffer.preswap();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube.Draw(cube_model_matrix, view_matrix, projection_matrix);
        quad.Draw(IDENTITY_MATRIX, view_matrix, projection_matrix);
    framebuffer.swap();
        screenquad.Draw(1);
    framebuffer.postSwap();
    //glViewport(0, 0, window_width, window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    screenquad.Draw(0);

    // TODO: use the fullscreen quad to draw the framebuffer texture to screen
    //       (see slides)

}

// gets called when the windows/framebuffer is resized.
void ResizeCallback(GLFWwindow* window, int width, int height) {
    window_width = width;
    window_height = height;

    float ratio = window_width / (float) window_height;
    projection_matrix = perspective(45.0f, ratio, 0.1f, 10.0f);

    glViewport(0, 0, window_width, window_height);

    // TODO : when the window is resized, the framebuffer and the fullscreen quad
    //        sizes should be updated accordingly
    framebuffer.Cleanup();
    framebuffer.Init(window_width, window_height);
    screenquad.UpdateSize(window_width, window_height);
}

void ErrorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        screenquad.changeStd(-0.25);
    } else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        screenquad.changeStd(0.25);
    }
}

int main(int argc, char *argv[]) {
    // GLFW Initialization
    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(ErrorCallback);

    // hint GLFW that we would like an OpenGL 3 context (at least)
    // http://www.glfw.org/faq.html#how-do-i-create-an-opengl-30-context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // attempt to open the window: fails if required version unavailable
    // note some Intel GPUs do not support OpenGL 3.2
    // note update the driver of your graphic card
    GLFWwindow* window = glfwCreateWindow(window_width, window_height,
                                          "framebuffer", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // makes the OpenGL context of window current on the calling thread
    glfwMakeContextCurrent(window);

    // set the callback for escape key
    glfwSetKeyCallback(window, KeyCallback);

    // set the framebuffer resize callback
    glfwSetFramebufferSizeCallback(window, ResizeCallback);

    // GLEW Initialization (must have a context)
    // https://www.opengl.org/wiki/OpenGL_Loading_Library
    glewExperimental = GL_TRUE; // fixes glew error (see above link)
    if(glewInit() != GLEW_NO_ERROR) {
        fprintf( stderr, "Failed to initialize GLEW\n");
        return EXIT_FAILURE;
    }

    cout << "OpenGL" << glGetString(GL_VERSION) << endl;

    // initialize our OpenGL program
    Init(window);

    // render loop
    while(!glfwWindowShouldClose(window)){
        Display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    quad.Cleanup();
    cube.Cleanup();
    // TODO: clean framebuffer and screenquad
    framebuffer.Cleanup();
    screenquad.Cleanup();

    // close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
