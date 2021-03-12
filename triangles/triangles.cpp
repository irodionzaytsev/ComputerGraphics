// check out the animation on youtube:
// https://youtu.be/kL9yIrlimOQ
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include <common/shader.hpp>

#include <iostream>
using namespace std;


int main() {
    srand(time(NULL));
    glewExperimental = true;
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW");
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1500, 1000, "Triangles", NULL, NULL);
    if (window == nullptr) {
        fprintf(stderr, "Failed to open GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "failed to initialize GLEW");
        return -1;
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint programID = LoadShaders( "../playground/homework/triangles/triangles.vertexshader",
                                     "../playground/homework/triangles/triangles.fragmentshader");

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
    };

    static GLfloat g_color_buffer_data_red[] = {
            1,0,0,
            1,0,0,
            1,0,0
    };

    static GLfloat g_color_buffer_data_green[] = {
            0,1,0,
            0,1,0,
            0,1,0
    };

    mat4 Projection = perspective(radians(45.0f), 4.0f/3.0f,.1f, 100.0f );
    float angle = 0;
    mat4 View = lookAt(vec3(cos(angle),sin(angle),4), vec3(0,0,0), vec3(0,1,0));
    mat4 Model1 = translate(vec3(0,0,1));
    mat4 Model2 = translate(vec3(0,0,0));
    mat4 mvp1 = Projection * View * Model1;
    mat4 mvp2 = Projection * View * Model2;

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof g_vertex_buffer_data, g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint redbuffer;
    glGenBuffers(1, &redbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, redbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof g_color_buffer_data_red, g_color_buffer_data_red, GL_STATIC_DRAW);

    GLuint greenbuffer;
    glGenBuffers(1, &greenbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, greenbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof g_color_buffer_data_green, g_color_buffer_data_green, GL_STATIC_DRAW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

        // move camera
        angle -= 0.02;
        mat4 View = lookAt(vec3(3*cos(angle), 3*sin(angle), 4), vec3(0,0,0), vec3(0,1,0));

        mvp2 = Projection * View * Model2;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp2[0][0]);


        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                0
        );
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, greenbuffer);
        glVertexAttribPointer(
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                0
        );
        glDrawArrays(GL_TRIANGLES, 0, 3);


        mvp1 = Projection * View * Model1;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp1[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                0
        );
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, redbuffer);
        glVertexAttribPointer(
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                0
        );
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));


}

