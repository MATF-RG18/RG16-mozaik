#include <cstdio>
#include "grid.hpp"

#include "mozaik_globals.hpp"

void create_grid(GLfloat vertex_buffer[], unsigned num_of_lines, glm::vec2 bottom_left, glm::vec2 top_right) {

    GLfloat x_spacing = (top_right.x - bottom_left.x) / (num_of_lines - 1);
    GLfloat y_spacing = (top_right.y - bottom_left.y) / (num_of_lines - 1);

    for (int i = 0; i < num_of_lines; i++) {
        // 4 vertices * attribute count
        int offset = ATTR_COUNT * i * 4;

        // Start point
        vertex_buffer[offset] = bottom_left.x + i * x_spacing;     // X
        vertex_buffer[offset + 1] = bottom_left.y;               // Y
        vertex_buffer[offset + 2] = 0.0f;                // Z
        vertex_buffer[offset + 3] = 1.0f;                // R
        vertex_buffer[offset + 4] = 1.0f;                // G
        vertex_buffer[offset + 5] = 1.0f;                // B

        // End point
        vertex_buffer[offset + 6] = bottom_left.x + i * x_spacing; // X
        vertex_buffer[offset + 7] = top_right.y;                // Y
        vertex_buffer[offset + 8] = 0.0f;                // Z
        vertex_buffer[offset + 9] = 1.0f;                // R
        vertex_buffer[offset + 10] = 1.0f;               // G
        vertex_buffer[offset + 11] = 1.0f;               // B

        // Start point
        vertex_buffer[offset + 12] = bottom_left.x;              // X
        vertex_buffer[offset + 13] = bottom_left.y + i * y_spacing;// Y
        vertex_buffer[offset + 14] = 0.0f;               // Z
        vertex_buffer[offset + 15] = 1.0f;               // R
        vertex_buffer[offset + 16] = 1.0f;               // G
        vertex_buffer[offset + 17] = 1.0f;               // B

        // End point
        vertex_buffer[offset + 18] = top_right.x;               // X
        vertex_buffer[offset + 19] = bottom_left.y + i * y_spacing;// Y
        vertex_buffer[offset + 20] = 0.0f;               // Z
        vertex_buffer[offset + 21] = 1.0f;               // R
        vertex_buffer[offset + 22] = 1.0f;               // G
        vertex_buffer[offset + 23] = 1.0f;               // B
    }
}

unsigned grid_vertex_count_hint(unsigned lines) {
    // num of lines * 2 sets of parallel lines * 2 vertices each * attribute count
    return lines * 2 * 2 * ATTR_COUNT;
}
