#ifndef TUTORIALS_TARGET_H
#define TUTORIALS_TARGET_H

void load_cube();
void load_carrot();

class target {
public:
    target();
    glm::vec3 get_coordinates();
    void draw(glm::mat4 VP);
private:
    glm::mat4 Model;
    glm::vec3 coordinates;
    enum class Shape {Carrot, Cube, size};
    Shape shape;
};


#endif //TUTORIALS_TARGET_H
