

#ifndef TUTORIALS_BULLET_H
#define TUTORIALS_BULLET_H

void load_bullet();
void shoot(GLFWwindow* window, int button, int action, int mods);
void motion(GLFWwindow* window, int key, int scancode, int action, int mods);
class bullet {
public:
    bullet();
    glm::vec3 get_coordinates(double current_time);
    void draw(glm::mat4 VP, double current_time);
    double get_initial_time();
    void explode();
    bool exploded();
private:
    glm::vec3 initial_position;
    glm::vec3 velocity;
    double initial_time;
    bool _exploded = false;
    const float explosion_factor = 1;
    double explosion_time;
};


#endif //TUTORIALS_BULLET_H
