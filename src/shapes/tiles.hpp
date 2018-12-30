#ifndef RG16_MOZAIK_TILES_HPP
#define RG16_MOZAIK_TILES_HPP

#include "shape.hpp"

class Tiles : public Shape {
public:
    Tiles(unsigned int num_of_lines, glm::vec2 start_vector, glm::vec2 end_vector);
private:
    void push_into_vertex_array(glm::vec2 vertex);
    void push_into_element_array(GLint first, GLint second, GLint third);

};

#endif //RG16_MOZAIK_TILES_HPP
