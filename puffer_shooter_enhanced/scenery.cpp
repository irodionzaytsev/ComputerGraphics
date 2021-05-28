//
// Created by rodion on 27.05.2021.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <list>
#include "controls.hpp"
#include <common/objloader.hpp>
#include <common/texture.hpp>

#include "scenery.h"
extern GLuint programID;
extern GLuint MatrixID;
extern GLuint TextureID;


GLuint Texture_sky;
std::vector<glm::vec3> vertices_sky;
std::vector<glm::vec2> uvs_sky;
GLuint vertexbuffer_sky;
GLuint uvbuffer_sky;
void load_sky() {
    Texture_sky = loadBMP_custom("../playground/homework/puffer_shooter_enhanced/sky.bmp");
    // Get a handle for our "textureSampler" uniform

    // Read our .obj file

    std::vector<glm::vec3> normals;
    loadOBJ("../playground/homework/puffer_shooter_enhanced/sky.obj", vertices_sky, uvs_sky, normals);

    // Load it into a VBO

    glGenBuffers(1, &vertexbuffer_sky);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_sky);
    glBufferData(GL_ARRAY_BUFFER, vertices_sky.size() * sizeof(glm::vec3), &vertices_sky[0], GL_STATIC_DRAW);


    glGenBuffers(1, &uvbuffer_sky);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_sky);
    glBufferData(GL_ARRAY_BUFFER, uvs_sky.size() * sizeof(glm::vec2), &uvs_sky[0], GL_STATIC_DRAW);
}
void draw_sky(glm::mat4 VP) {
    glm::mat4 MVP = VP * glm::translate(glm::mat4(), position);
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture_sky);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(TextureID, 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_sky);
    glVertexAttribPointer(
            0,                  // attribute
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) 0            // array buffer offset
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_sky);
    glVertexAttribPointer(
            1,                                // attribute
            2,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void *) 0                          // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, vertices_sky.size());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
GLuint Texture_floor;
static const GLfloat vertices_floor[] = {
        1,0,0,
        0,0,0,
        0,0,1,

        1,0,1,
        1,0,0,
        0,0,1
};
static const GLfloat uvs_floor[] = {
        1,0,
        0,0,
        0,1,

        1,1,
        1,0,
        0,1
};
GLuint vertexbuffer_floor;
GLuint uvbuffer_floor;

void load_floor() {
    Texture_floor = loadBMP_custom("../playground/homework/puffer_shooter_enhanced/stone.bmp");
    // Get a handle for our "textureSampler" uniform

    // Read our .obj file

    // Load it into a VBO

    glGenBuffers(1, &vertexbuffer_floor);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_floor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_floor), &vertices_floor[0], GL_STATIC_DRAW);


    glGenBuffers(1, &uvbuffer_floor);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_floor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs_floor), &uvs_floor[0], GL_STATIC_DRAW);
}
const int FLOOR_SIZE = 10;
void draw_floor(glm::mat4 VP) {
    glm::tvec3<float> integer_position =  (glm::tvec3<float>)  (glm::tvec3<int>) position;
    /*integer_position[0] = (int) position[0];
    integer_position[1] = (int) position[1];
    integer_position[2] = (int) position[2];*/
    for (int i = -FLOOR_SIZE; i <= FLOOR_SIZE; ++i) {
        for (int j = -FLOOR_SIZE; j <= FLOOR_SIZE; ++j) {
            glm::tvec3<float> center;
            center[0] = integer_position[0] + i;
            center[1] = -1;
            center[2] = integer_position[2] + j;
            glm::mat4 MVP = VP * glm::translate(glm::mat4(), center);
            // Send our transformation to the currently bound shader,
            // in the "MVP" uniform
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, Texture_floor);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(TextureID, 0);

            // 1rst attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_floor);
            glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void *) 0            // array buffer offset
            );

            // 2nd attribute buffer : UVs
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_floor);
            glVertexAttribPointer(
                    1,                                // attribute
                    2,                                // size
                    GL_FLOAT,                         // type
                    GL_FALSE,                         // normalized?
                    0,                                // stride
                    (void *) 0                          // array buffer offset
            );

            // Draw the triangle !
            glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices_floor));

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
        }
    }

}