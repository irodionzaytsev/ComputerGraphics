#include <stdlib.h>

#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include <vector>
#include "controls.hpp"
#include <common/objloader.hpp>
#include <common/texture.hpp>

#include "target.h"

const float MAX_DISTANCE = 10.0f;

extern GLuint programID;
extern GLuint MatrixID;
extern GLuint TextureID;


GLuint Texture_cube;
std::vector<glm::vec3> vertices_cube;
std::vector<glm::vec2> uvs_cube;
GLuint vertexbuffer_cube;
GLuint uvbuffer_cube;
void load_cube() {
    Texture_cube = loadDDS("../playground/homework/puffer_shooter/uvmap.DDS");
    // Get a handle for our "textureSampler" uniform

    // Read our .obj file

    std::vector<glm::vec3> normals;
    loadOBJ("../playground/homework/puffer_shooter/cube.obj", vertices_cube, uvs_cube, normals);

    // Load it into a VBO

    glGenBuffers(1, &vertexbuffer_cube);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_cube);
    glBufferData(GL_ARRAY_BUFFER, vertices_cube.size() * sizeof(glm::vec3), &vertices_cube[0], GL_STATIC_DRAW);


    glGenBuffers(1, &uvbuffer_cube);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_cube);
    glBufferData(GL_ARRAY_BUFFER, uvs_cube.size() * sizeof(glm::vec2), &uvs_cube[0], GL_STATIC_DRAW);
}

GLuint Texture_carrot;
std::vector<glm::vec3> vertices_carrot;
std::vector<glm::vec2> uvs_carrot;
GLuint vertexbuffer_carrot;
GLuint uvbuffer_carrot;
void load_carrot() {
    Texture_carrot = loadBMP_custom("../playground/homework/puffer_shooter/carrot.bmp");
    // Get a handle for our "textureSampler" uniform

    // Read our .obj file

    std::vector<glm::vec3> normals;
    loadOBJ("../playground/homework/puffer_shooter/carrot.obj", vertices_carrot, uvs_carrot, normals);

    // Load it into a VBO

    glGenBuffers(1, &vertexbuffer_carrot);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_carrot);
    glBufferData(GL_ARRAY_BUFFER, vertices_carrot.size() * sizeof(glm::vec3), &vertices_carrot[0], GL_STATIC_DRAW);


    glGenBuffers(1, &uvbuffer_carrot);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_carrot);
    glBufferData(GL_ARRAY_BUFFER, uvs_carrot.size() * sizeof(glm::vec2), &uvs_carrot[0], GL_STATIC_DRAW);
}

glm::vec3 generate_random_coordinates() {
    float x = MAX_DISTANCE * (float(rand()) / float(RAND_MAX) - 2.0f);
    float y = abs(MAX_DISTANCE * (float(rand()) / float(RAND_MAX) - 2.0f));
    float z = MAX_DISTANCE * (float(rand()) / float(RAND_MAX) - 2.0f);
    return glm::vec3(x,y,z);
}
target::target() {
    shape = Shape(rand() % int(Shape::size));
    coordinates = position + generate_random_coordinates();
    glm::vec3 axis = generate_random_coordinates();
    float angle = float(rand());
    Model = glm::translate(coordinates) * glm::rotate(angle, axis);}

glm::vec3 target::get_coordinates() { return coordinates; }

void target::draw(glm::mat4 VP) {
    glm::mat4 MVP = VP * Model;
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    switch (shape) {
        case Shape::Cube:
            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, Texture_cube);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(TextureID, 0);

            // 1rst attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_cube);
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
            glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_cube);
            glVertexAttribPointer(
                    1,                                // attribute
                    2,                                // size
                    GL_FLOAT,                         // type
                    GL_FALSE,                         // normalized?
                    0,                                // stride
                    (void *) 0                          // array buffer offset
            );

            // Draw the triangle !
            glDrawArrays(GL_TRIANGLES, 0, vertices_cube.size());

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            break;
        case Shape::Carrot:
            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, Texture_carrot);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(TextureID, 0);

            // 1rst attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_carrot);
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
            glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_carrot);
            glVertexAttribPointer(
                    1,                                // attribute
                    2,                                // size
                    GL_FLOAT,                         // type
                    GL_FALSE,                         // normalized?
                    0,                                // stride
                    (void *) 0                          // array buffer offset
            );

            // Draw the triangle !
            glDrawArrays(GL_TRIANGLES, 0, vertices_carrot.size());

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);

    }
}