#include "color_sphere.hpp"
#include "../mozaik_globals.hpp"

static unsigned triangle_vertex_array_size_hint(unsigned lod);

ColorSphere::ColorSphere(unsigned lod) {
    // Number of vertices per one octahedron side * 8 sides of an octahedron
    vertex_data_size =
            triangle_vertex_array_size_hint(lod) * 8 * ATTR_COUNT * sizeof(GLfloat);
    // TODO: visual explanation
    element_data_size = static_cast<GLsizei>((pow(lod - 1, 2) * 3 * 8) * sizeof(GLint));
}

unsigned triangle_vertex_array_size_hint(unsigned lod) {
    // Sum of the first lod numbers
    // (see https://github.com/MATF-RG18/RG16-mozaik/wiki/Miscellaneous-code-and-geometry-explanations)
    return (lod * (lod + 1) / 2);
}
