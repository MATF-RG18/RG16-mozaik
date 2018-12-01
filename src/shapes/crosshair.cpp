#include <algorithm>
#include <cstdio>
#include "crosshair.hpp"
#include "../mozaik_globals.hpp"

Crosshair::Crosshair() {
    vertex_data_size = 4 * ATTR_COUNT * sizeof(GLfloat);
    GLfloat vertex_data_array[] = {
            0.0f, -0.02f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.0f, 0.02f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.02f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.02f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    };

    vertex_data = new GLfloat[4 * ATTR_COUNT];

    for (int i = 0; i < 4 * ATTR_COUNT; ++i) {
        vertex_data[i] = vertex_data_array[i];
    }
}
