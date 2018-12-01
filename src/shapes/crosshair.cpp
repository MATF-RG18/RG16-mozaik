#include "crosshair.hpp"
#include "../mozaik_globals.hpp"

Crosshair::Crosshair() {
    vertex_data_size = 4 * ATTR_COUNT * sizeof(GLfloat);
}
