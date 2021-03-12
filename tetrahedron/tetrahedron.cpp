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

void color(GLfloat *vertices, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < 3; ++j) {
            vertices[i * 3 + j] = (float) rand() / (float) RAND_MAX;
        }
    }
}
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
    GLFWwindow* window = glfwCreateWindow(2*1024, 2*769, "Tetrahedron", NULL, NULL);
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

    GLuint programID = LoadShaders( "../playground/homework/tetrahedron/tetrahedron.vertexshader",
                                    "../playground/homework/tetrahedron/tetrahedron.fragmentshader" );
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    static const GLfloat g_vertex_buffer_data[] = {
            1,0,0, // first triangle
            0,1,0,
            0,0,1, // end first triangle

            0,0,0,
            1,0,0,
            0,1,0,

            0,0,0,
            1,0,0,
            0,0,1,

            0,0,0,
            0,1,0,
            0,0,1

    };
    GLfloat g_color_buffer_data[] = {
            0.583f,  0.771f,  0.014f,
            0.609f,  0.115f,  0.436f,
            0.327f,  0.483f,  0.844f,
            0.822f,  0.569f,  0.201f,
            0.435f,  0.602f,  0.223f,
            0.310f,  0.747f,  0.185f,
            0.597f,  0.770f,  0.761f,
            0.559f,  0.436f,  0.730f,
            0.359f,  0.583f,  0.152f,
            0.483f,  0.596f,  0.789f,
            0.559f,  0.861f,  0.639f,
            0.195f,  0.548f,  0.859f,
    };
    color(g_color_buffer_data, 4);

    mat4 Projection = perspective(radians(45.0f), 4.0f/3.0f,.1f, 100.0f );
    float angle = 0;
    mat4 View = lookAt(vec3(cos(angle),sin(angle),2), vec3(0,0,0), vec3(0,1,0));
    mat4 Model = translate(vec3(0,-0.7,0)); //rotate(radians(70.f), vec3(1,0,0));
    mat4 mvp = Projection * View * Model;

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof g_vertex_buffer_data, g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof g_color_buffer_data, g_color_buffer_data, GL_STATIC_DRAW);


    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        // move camera
        angle -= 0.02;
        mat4 View = lookAt(vec3(3*cos(angle), 0, 3*sin(angle)), vec3(0,0,0), vec3(0,1,0));
        mvp = Projection * View * Model;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);


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
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                0
        );
        glDrawArrays(GL_TRIANGLES, 0, 12*3);


        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));


}