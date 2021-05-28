#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unistd.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

GLFWwindow* window;
#include <common/shader.hpp>
#include <common/texture.hpp>
#include "controls.hpp"
#include <common/objloader.hpp>
#include "target.h"
#include "bullet.h"
#include "scenery.h"
#include <iostream>
using namespace std;

GLuint programID;
GLuint MatrixID;
GLuint TextureID;
list<bullet> bullets;

int main() {
    const double MAX_TIME = 10;
    const double MIN_DIST = 2;
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Puffer shooter", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders( "../playground/homework/puffer_shooter/TransformVertexShader.vertexshader",
                             "../playground/homework/puffer_shooter/TextureFragmentShader.fragmentshader" );
    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");
    TextureID  = glGetUniformLocation(programID, "textureSampler");

    load_cube();
    load_carrot();
    load_bullet();
    load_sky();
    load_floor();
    list<target> targets;
    targets.push_back(target());
    glfwSetMouseButtonCallback(window, shoot);
    glfwSetKeyCallback(window, reinterpret_cast<GLFWkeyfun>(motion));
    const double CREATE_TARGET_INTERVAL = 5.0;
    double lastTime = glfwGetTime();
    double currentTime;
    do {
        // create new target if needed
        currentTime = glfwGetTime();
        if (currentTime - lastTime > CREATE_TARGET_INTERVAL) {
            targets.push_back(target());
            lastTime = currentTime;
        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 VP = ProjectionMatrix * ViewMatrix;
        draw_sky(VP);
        draw_floor(VP);
        for (target& t : targets) {
            t.draw(VP);
        }
        for (bullet& b : bullets) {
            b.draw(VP, currentTime);
        }

        // delete objects as needed
        for (auto b = bullets.begin(); b != bullets.end();) {
            if (currentTime - b->get_initial_time() > MAX_TIME) {
                b = bullets.erase(b);
                continue;
            }
            bool del = false;
            for (auto t = targets.begin(); t != targets.end();) {
                if (!b->exploded() && glm::distance(b->get_coordinates(currentTime), t->get_coordinates()) < MIN_DIST) {
                    del = true;
                    t = targets.erase(t);
                } else ++t;
            }
            if (del) {
                b->explode();
            } else ++b;
        }

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );


}
