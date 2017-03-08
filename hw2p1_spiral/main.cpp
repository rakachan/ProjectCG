// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#include "icg_helper.h"

#include <glm/gtc/matrix_transform.hpp>

#include "triangle/triangle.h"

#define NB_TRIANGLES 500
#define FERMAT false

Triangle triangle;

glm::mat4 rotation(float alpha) {
    return glm::mat4(glm::vec4(cos(alpha), sin(alpha), 0.0, 0.0),
                     glm::vec4(-sin(alpha), cos(alpha) , 0.0, 0.0),
                     glm::vec4(0.0       , 0.0        , 1.0, 0.0),
                     glm::vec4(0.0       , 0.0        , 0.0, 1.0));
}

glm::mat4 translation(float tx, float ty) {
    return glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0),
                     glm::vec4(0.0, 1.0, 0.0, 0.0),
                     glm::vec4(0.0, 0.0, 1.0, 0.0),
                     glm::vec4(tx, ty, 0.0, 1.0));
}

glm::mat4 scale(float sx, float sy) {
    return glm::mat4(glm::vec4(sx, 0.0, 0.0, 0.0),
                     glm::vec4(0.0, sy, 0.0, 0.0),
                     glm::vec4(0.0, 0.0, 1.0, 0.0),
                     glm::vec4(0.0, 0.0, 0.0, 1.0));
}

void Init() {
    // sets background color
    glClearColor(0.937f, 0.937f, 0.937f /*gray*/, 1.0f /*solid*/);

    triangle.Init();
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (FERMAT) {
        float c = 0.04f;

        for(int i = 0; i< NB_TRIANGLES; i++) {
            float r = c * sqrt(i);
            float theta = i * 137.508f;
            triangle.Draw(rotation(theta) * translation(r, 0) * scale(c/2, c/2));
        }
    }
    else {
        // compute transformations here
        float angle_modif = 0.27f;
        float translate_modif = 0.008f;
        float scale_modif = 0.01f;

        for(int i = 0; i< NB_TRIANGLES/5; i++) {
            triangle.Draw(rotation(i*angle_modif) * translation(i*translate_modif, 0) * scale((float(i)/10)*scale_modif, ((float)(i)/10)*scale_modif));
        }
    }
}

void ErrorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
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
    GLFWwindow* window = glfwCreateWindow(512, 512, "spiral", NULL, NULL);
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

    triangle.Cleanup();

    // close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

