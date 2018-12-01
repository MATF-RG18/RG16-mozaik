#include "lines.hpp"
#include "../mozaik_globals.hpp"

Lines::Lines() {
    vertex_data_size = 1000 * ATTR_COUNT * sizeof(GLfloat);
    element_data_size = -1;
}