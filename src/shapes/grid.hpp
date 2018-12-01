#ifndef RG16_MOZAIK_GRID_HPP
#define RG16_MOZAIK_GRID_HPP

#include "shape.hpp"

class Grid : public Shape {
private:
    unsigned num_of_lines;
    glm::vec2 start_vector;
    glm::vec2 end_vector;
public:
    Grid(unsigned int num_of_lines, glm::vec2 start_vector, glm::vec2 end_vector, glm::mat4 model_matrix = glm::mat4(1.0f));
};


#endif //RG16_MOZAIK_GRID_HPP
