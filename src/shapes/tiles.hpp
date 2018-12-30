#ifndef RG16_MOZAIK_TILES_HPP
#define RG16_MOZAIK_TILES_HPP

#include "shape.hpp"

class Tiles : public Shape {
public:
    Tiles(unsigned int num_of_lines, glm::vec3 start_vector, glm::vec3 end_vector);

    void color_a_tile(glm::vec3 position, const glm::vec3 &color);
private:
    unsigned num_of_tiles;
    glm::vec3 start_vector;
    glm::vec3 end_vector;

    // Size of the individual quad tile
    GLfloat x_size;
    GLfloat y_size;

    void push_into_vertex_array(glm::vec2 vertex);
    void push_into_element_array(GLint first, GLint second, GLint third);

};

#endif //RG16_MOZAIK_TILES_HPP
