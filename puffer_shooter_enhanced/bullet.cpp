#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <list>
#include "controls.hpp"
#include <common/objloader.hpp>
#include <common/texture.hpp>

#include "bullet.h"

extern GLuint programID;
extern GLuint MatrixID;
extern GLuint TextureID;
extern std::list<bullet> bullets;

GLuint Texture_bullet;
std::vector<glm::vec3> vertices_bullet;
std::vector<glm::vec2> uvs_bullet;
std::vector<glm::tvec3<float>> normals_bullet;
GLuint vertexbuffer_bullet;
GLuint uvbuffer_bullet;
float bullet_speed;
void load_bullet() {
    bullet_speed = 20.0;
    Texture_bullet = loadBMP_custom("../playground/homework/puffer_shooter_enhanced/puffer.bmp");
    // Get a handle for our "textureSampler" uniform

    // Read our .obj file
    std::vector<glm::vec3> normals;
    loadOBJ("../playground/homework/puffer_shooter_enhanced/puffer.obj", vertices_bullet, uvs_bullet, normals);
    for (auto& normal : normals) {
        normals_bullet.push_back(glm::tvec3<float>(normal));
    }
    // Load it into a VBO

    glGenBuffers(1, &vertexbuffer_bullet);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_bullet);
    glBufferData(GL_ARRAY_BUFFER, vertices_bullet.size() * sizeof(glm::vec3), &vertices_bullet[0], GL_STATIC_DRAW);


    glGenBuffers(1, &uvbuffer_bullet);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_bullet);
    glBufferData(GL_ARRAY_BUFFER, uvs_bullet.size() * sizeof(glm::vec2), &uvs_bullet[0], GL_STATIC_DRAW);
}

bullet::bullet() {
    initial_position = position;
    velocity = direction * bullet_speed;
    initial_time = glfwGetTime();
}

glm::vec3 bullet::get_coordinates(double current_time) {
    return initial_position + float(current_time - initial_time) * velocity;
}
double bullet::get_initial_time() {
    return initial_time;
}

void bullet::draw(glm::mat4 VP, double current_time) {
    if (!exploded()) {
        glm::mat4 MVP = VP * glm::translate(glm::mat4(),
                                            initial_position + float(current_time - initial_time) * velocity);
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture_bullet);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_bullet);
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
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_bullet);
        glVertexAttribPointer(
                1,                                // attribute
                2,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void *) 0                          // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, vertices_bullet.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    } else {
        for (int i = 0; i < vertices_bullet.size(); ++i) {
            glm::mat4 MVP = VP * glm::translate(glm::mat4(),
                                                initial_position +
                                                float(explosion_time - initial_time) * velocity
                                                + float(current_time - explosion_time)
                                                * bullet_speed * explosion_factor * (normals_bullet[i]));
            // Send our transformation to the currently bound shader,
            // in the "MVP" uniform
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, Texture_bullet);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(TextureID, 0);

            // 1rst attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_bullet);
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
            glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_bullet);
            glVertexAttribPointer(
                    1,                                // attribute
                    2,                                // size
                    GL_FLOAT,                         // type
                    GL_FALSE,                         // normalized?
                    0,                                // stride
                    (void *) 0                          // array buffer offset
            );

            // Draw the triangle !
            glDrawArrays(GL_TRIANGLES, i, 3);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);

        }
    }
}
void shoot(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        bullets.push_back(bullet());
    }
}
void motion(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        bullet_speed += 5;
        return;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        if (bullet_speed == 1) return;
        if (bullet_speed <= 5) bullet_speed -= 1;
        else bullet_speed -= 5;
    }
}

void bullet::explode() {
    _exploded = true;
    explosion_time = glfwGetTime();
}
bool bullet::exploded() {
    return _exploded;
}