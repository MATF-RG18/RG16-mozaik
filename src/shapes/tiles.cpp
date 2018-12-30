#include "tiles.hpp"
#include "../mozaik_globals.hpp"

static glm::vec2 multiply(glm::vec2 vector, float factor);

/* TODO documentation */
Tiles::Tiles(unsigned int num_of_tiles, glm::vec2 start_vector, glm::vec2 end_vector)
    : Shape(glm::mat4(1.0f), GL_TRIANGLES, TILES) {

    GLfloat x_spacing = (end_vector.x - start_vector.x) / num_of_tiles;
    GLfloat y_spacing = (end_vector.y - start_vector.y) / num_of_tiles;

    // Number of vertices per side
    unsigned n = num_of_tiles + 1;

    vertex_data_size = n * n * ATTR_COUNT * sizeof(GLfloat);
    vertex_data = new GLfloat[n * n * ATTR_COUNT];

    // (n-1)*(n-1) quads tiles = (n-1)*(n-1) * 2 triangle tiles = (n-1)*(n-1) * 2 * 3 indices
    element_data_size = (n - 1) * (n - 1) * 2 * 3 * sizeof(GLint);
    element_data = new GLint[(n - 1) * (n - 1) * 2 * 3];

    // Base vectors
    glm::vec2 e1(x_spacing, 0.0);
    glm::vec2 e2(0.0, y_spacing);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            push_into_vertex_array(start_vector + multiply(e1, i) + multiply(e2, j));

            // there are (n-1) * (n-1) tiles
            if (i != n - 1 && j != n - 1) {
                // Top triangle
                GLint first = i + j * n;
                GLint second = i + (j + 1) * n;
                GLint third = i + (j + 1) * n + 1;
                push_into_element_array(first, second, third);
                // Bottom triangle
                first = i + j * n;
                second = i + j * n + 1;
                third = i + (j + 1) * n + 1;
                push_into_element_array(first, second, third);
            }
        }
    }

    // TODO create triangles by using element buffer
}

void Tiles::push_into_vertex_array(glm::vec2 vertex) {
    static unsigned offset;

    vertex_data[offset] = vertex.x;
    vertex_data[offset + 1] = vertex.y;
    vertex_data[offset + 2] = 0.0f;
    vertex_data[offset + 3] = 1.0f;
    vertex_data[offset + 4] = 1.0f;
    vertex_data[offset + 5] = 1.0f;

    offset += ATTR_COUNT;
}

void Tiles::push_into_element_array(GLint first, GLint second, GLint third) {
    static unsigned offset;

    element_data[offset] = first;
    element_data[offset + 1] = second;
    element_data[offset + 2] = third;

    offset += 3;
}

glm::vec2 multiply(glm::vec2 vector, float factor) {
    vector.x *= factor;
    vector.y *= factor;
    return vector;
}
