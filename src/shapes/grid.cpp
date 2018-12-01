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
    vertex_data = new GLfloat[num_of_lines * 2 * 2 * ATTR_COUNT];
    vertex_data_size = num_of_lines * 2 * 2 * ATTR_COUNT * sizeof(GLfloat);

    GLfloat x_spacing = (end_vector.x - start_vector.x) / (num_of_lines - 1);
    GLfloat y_spacing = (end_vector.y - start_vector.y) / (num_of_lines - 1);

    for (int i = 0; i < num_of_lines; i++) {
        // 4 vertices * attribute count
        int offset = ATTR_COUNT * i * 4;

        // Start point
        vertex_data[offset] = start_vector.x + i * x_spacing;     // X
        vertex_data[offset + 1] = start_vector.y;               // Y
        vertex_data[offset + 2] = 0.0f;                // Z
        vertex_data[offset + 3] = 0.7f;                // R
        vertex_data[offset + 4] = 0.7f;                // G
        vertex_data[offset + 5] = 0.7f;                // B

        // End point
        vertex_data[offset + 6] = start_vector.x + i * x_spacing; // X
        vertex_data[offset + 7] = end_vector.y;                // Y
        vertex_data[offset + 8] = 0.0f;                // Z
        vertex_data[offset + 9] = 0.7f;                // R
        vertex_data[offset + 10] = 0.7f;               // G
        vertex_data[offset + 11] = 0.7f;               // B

        // Start point
        vertex_data[offset + 12] = start_vector.x;              // X
        vertex_data[offset + 13] = start_vector.y + i * y_spacing;// Y
        vertex_data[offset + 14] = 0.0f;               // Z
        vertex_data[offset + 15] = 0.7f;               // R
        vertex_data[offset + 16] = 0.7f;               // G
        vertex_data[offset + 17] = 0.7f;               // B

        // End point
        vertex_data[offset + 18] = end_vector.x;               // X
        vertex_data[offset + 19] = start_vector.y + i * y_spacing;// Y
        vertex_data[offset + 20] = 0.0f;               // Z
        vertex_data[offset + 21] = 0.7f;               // R
        vertex_data[offset + 22] = 0.7f;               // G
        vertex_data[offset + 23] = 0.7f;               // B
    }

}
