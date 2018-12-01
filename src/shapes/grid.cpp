#include "grid.hpp"
#include "../mozaik_globals.hpp"

Grid::Grid(
        unsigned int num_of_lines,
        glm::vec2 start_vector,
        glm::vec2 end_vector,
        glm::mat4 model_matrix) : num_of_lines(num_of_lines),
                                  start_vector(start_vector),
                                  end_vector(end_vector) {

    this->model_matrix = model_matrix;
    draw_mode = GL_LINES;
    // num of lines * 2 sets of parallel lines * 2 vertices each * attribute count
    vertex_data_size = num_of_lines * 2 * 2 * ATTR_COUNT * sizeof(GLfloat);
    element_data_size = -1;

}
