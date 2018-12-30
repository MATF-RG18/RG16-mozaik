#include "tiles.hpp"
#include "../mozaik_globals.hpp"

/* TODO documentation */
Tiles::Tiles(unsigned int num_of_tiles, glm::vec3 start_vector, glm::vec3 end_vector)
    : Shape(glm::mat4(1.0f), GL_TRIANGLES, TILES),
      num_of_tiles(num_of_tiles),
      start_vector(start_vector),
      end_vector(end_vector) {

    x_size = (end_vector.x - start_vector.x) / num_of_tiles;
    y_size = (end_vector.y - start_vector.y) / num_of_tiles;

    // Number of vertices per side
    unsigned n = num_of_tiles + 1;

    vertex_data_size = n * n * ATTR_COUNT * sizeof(GLfloat);
    vertex_data = new GLfloat[n * n * ATTR_COUNT];

    // (n-1)*(n-1) quads tiles = (n-1)*(n-1) * 2 triangle tiles = (n-1)*(n-1) * 2 * 3 indices
    element_data_size = (n - 1) * (n - 1) * 2 * 3 * sizeof(GLint);
    element_data = new GLint[(n - 1) * (n - 1) * 2 * 3];

    // Base vectors
    glm::vec3 e1(x_size, 0.0f, 0.0f);
    glm::vec3 e2(0.0f, y_size, 0.0f);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            push_into_vertex_array(start_vector + e1 * (float) j + e2 * (float) i);

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
}

void Tiles::push_into_vertex_array(glm::vec2 vertex) {
    static unsigned offset;

    vertex_data[offset] = vertex.x;
    vertex_data[offset + 1] = vertex.y;
    // A little bit less than zero, so it doesn't interfere with the Grid.
    vertex_data[offset + 2] = -0.01f;
    vertex_data[offset + 3] = 0.0f;
    vertex_data[offset + 4] = 0.0f;
    vertex_data[offset + 5] = 0.0f;

    offset += ATTR_COUNT;
}

void Tiles::push_into_element_array(GLint first, GLint second, GLint third) {
    static unsigned offset;

    element_data[offset] = first;
    element_data[offset + 1] = second;
    element_data[offset + 2] = third;

    offset += 3;
}

void Tiles::color_a_tile(glm::vec3 position, const glm::vec3 &color) {
    // Determine a tile quad based on the coordinates
    // Translate the position vertex to the start vertex (adjust for the coordinate system difference)
    position += (end_vector - start_vector) / 2.0f;

    // Gives the grid position of the bottom-left index
    int x_pos = static_cast<int>(floor(position.x / x_size));
    int y_pos = static_cast<int>(floor(position.y / y_size));

    if (x_pos < 0 || x_pos >= num_of_tiles || y_pos < 0 || y_pos >= num_of_tiles) {
        // The point is not within bounds
        return;
    }

    // Array index of the bottom left vertex of the tile quad
    int bottom_left_index = (x_pos + (num_of_tiles + 1) * y_pos);
    // Array index of the top right vertex of the tile quad (the color-provoking vertex)
    int top_right_index = bottom_left_index + 1 + (num_of_tiles + 1);

    // glm::vec3 can't be used directly
    GLfloat color_array[3];
    color_array[0] = color.r;
    color_array[1] = color.g;
    color_array[2] = color.b;

    glBufferSubData(GL_ARRAY_BUFFER,
            // "+ 3" is to offset the pointer to the R,G,B part of the vertex, instead of the X,Y,Z
            vertex_buffer_offset + (top_right_index * ATTR_COUNT + 3) * sizeof(GLfloat),
            3 * sizeof(GLfloat), color_array);
}
