#include "grid.hpp"

#define ATTR_COUNT 6

void create_grid(GLfloat vertex_buffer[], unsigned num_of_lines) {

    GLfloat spacing = 2.0f / (num_of_lines - 1);

    for (int i = 0; i < num_of_lines; i++) {
        // 4 vertices * attribute count
        int offset = ATTR_COUNT * i * 4;

        // Start point
        vertex_buffer[offset] = -1.0f + i * spacing;     // X
        vertex_buffer[offset + 1] = -1.0f;               // Y
        vertex_buffer[offset + 2] = 0.0f;                // Z
        vertex_buffer[offset + 3] = 1.0f;                // R
        vertex_buffer[offset + 4] = 1.0f;                // G
        vertex_buffer[offset + 5] = 1.0f;                // B

        // End point
        vertex_buffer[offset + 6] = -1.0f + i * spacing; // X
        vertex_buffer[offset + 7] = 1.0f;                // Y
        vertex_buffer[offset + 8] = 0.0f;                // Z
        vertex_buffer[offset + 9] = 1.0f;                // R
        vertex_buffer[offset + 10] = 1.0f;               // G
        vertex_buffer[offset + 11] = 1.0f;               // B

        // Start point
        vertex_buffer[offset + 12] = -1.0f;              // X
        vertex_buffer[offset + 13] = -1.0f + i * spacing;// Y
        vertex_buffer[offset + 14] = 0.0f;               // Z
        vertex_buffer[offset + 15] = 1.0f;               // R
        vertex_buffer[offset + 16] = 1.0f;               // G
        vertex_buffer[offset + 17] = 1.0f;               // B

        // End point
        vertex_buffer[offset + 18] = 1.0f;               // X
        vertex_buffer[offset + 19] = -1.0f + i * spacing;// Y
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
