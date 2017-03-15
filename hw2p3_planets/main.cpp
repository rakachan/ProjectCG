// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#include "icg_helper.h"

#include "quad/quad.h"

// Quad stuff1;
Quad sun;
Quad earth;
Quad moon;
float speed = 50;
float speedIncrement = speed/10;

void Init() {
    // sets background color
    glClearColor(1.0,1.0,1.0 /*white*/, 1.0 /*solid*/);
    // {stuff}.Init(...);
    sun.Init("sun.tga");
    earth.Init("earth.tga");
    moon.Init("moon.tga");
}


glm::mat4 S(float s) {
    return glm::mat4(glm::mat3(s));
}

glm::mat4 R (float r) {
    // rotation
    glm::mat3 R = glm::mat3(1);
    float alpha = r;
    R[0][0] =  cos(alpha);
    R[0][1] =  sin(alpha);
    R[1][0] = -sin(alpha);
    R[1][1] =  cos(alpha);
    return glm::mat4(R);
}

glm::mat4 T (float x, float y) {
    // translation
    glm::mat4 T = glm::mat4(1);
    T[3][0] = x;
    T[3][1] = y;
    return glm::mat4(T);
}

glm::mat4 E(float a, float b, float angle) {
    // rotation and variable translation
    glm::mat4 rot = R(angle);
    rot[3][0] =  -a*cos(angle);
    rot[3][1] =  -b*sin(angle);
    return glm::mat4(rot);
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    // speed can be changed using the left and right arrow keys
    float time_s = speed * glfwGetTime();
    float moonSpeed = time_s/27.3;
    float sunSpeed = time_s/38;
    float earthSpeed = time_s;
    float revolutionSpeed = time_s/365;

    // compute the transformation matrices
    glm::mat4 revolution = E(0, 0.3, revolutionSpeed) * T(0.8, 0);
    sun.Draw(T(0.35,0) * R(sunSpeed) * S(0.2));
    earth.Draw(revolution * R(earthSpeed) * S(0.08));
    moon.Draw(revolution * R(moonSpeed) * T(0.2, 0) /** R(-moonSpeed)*/ * S(0.03));
}

void ErrorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        speed -= speedIncrement;
        if (speed < 0) {
            speed = 0;
        }
    } else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        speed += speedIncrement;
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
    GLFWwindow* window = glfwCreateWindow(512, 512, "planets", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // makes the OpenGL context of window current on the calling thread
    glfwMakeContextCurrent(window);

    // set the callback for escape key
    glfwSetKeyCallback(window, KeyCallback);

    // GLEW Initialization (must have a context)
    // https://www.opengl.org/wiki/OpenGL_Loading_Library
    glewExperimental = GL_TRUE; // fixes glew error (see above link)
    if(glewInit() != GLEW_NO_ERROR) {
        fprintf( stderr, "Failed to initialize GLEW\n");
        return EXIT_FAILURE;
    }

    cout << "OpenGL" << glGetString(GL_VERSION) << endl;

    // initialize our OpenGL program
    Init();

    // render loop
    while(!glfwWindowShouldClose(window)) {
        Display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // {stuff}.Cleanup()
    sun.Cleanup();
    earth.Cleanup();
    moon.Cleanup();

    // close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
