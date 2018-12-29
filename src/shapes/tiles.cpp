#include "tiles.hpp"
#include "../mozaik_globals.hpp"

Tiles::Tiles() {
    model_matrix = glm::mat4(1.0f);
    // TEST DATA
    GLfloat vertex_data_array[] = {
            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
    };

    vertex_data = new GLfloat[3 * ATTR_COUNT];

    for (int i = 0; i < 3 * ATTR_COUNT; ++i) {
        vertex_data[i] = vertex_data_array[i];
    }

    vertex_data_size = 3 * ATTR_COUNT * sizeof(GLfloat);


}
