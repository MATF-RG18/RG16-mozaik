#include "color_sphere.hpp"
#include <math.h>
#include <cstdio>

// TODO: define ATTR_COUNT in a single place.
#define ATTR_COUNT 6

/** Normalize the distance from the center to length
 *
 * This function modifies the original vertex in-place.
 *
 * @param center
 * The center from which the distance is measured.
 * @param orig
 * The vertex which will be modified in-place.
 * @param length
 * The length to which the distance between vertices should be normalized.
 */
static void normalize(glm::vec3 &vector, GLfloat length);

static void put_into_array(GLfloat* vertex_array, unsigned offset, glm::vec3 vertex);

void create_sphere(GLfloat vertex_array[], GLuint element_array[], GLfloat radius, unsigned lod) {

    // The point from which all other points will be translated
    glm::vec3 origin = glm::vec3(1.0f, 0.0f, 0.0f);

    glm::vec3 base1 = glm::vec3(-1.0f, 1.0f, 0.0f);
    // Normalize to match length of a single distance between vertices
    normalize(base1, 1.0f / lod);

    glm::vec3 base2 = glm::vec3(-1.0f, 0.0f, 1.0f);
    // Normalize to match length of a single distance between vertices
    normalize(base2, 1.0f / lod);

    // Test data
    put_into_array(vertex_array, 0, origin);
    put_into_array(vertex_array, 1, origin + base1);
    put_into_array(vertex_array, 2, origin + base2);
    put_into_array(vertex_array, 3, origin + base1 + base1);

    element_array[0] = 0;
    element_array[1] = 1;
    element_array[2] = 2;
    element_array[3] = 1;
    element_array[4] = 2;
    element_array[5] = 3;
}

void normalize(glm::vec3 &vector, GLfloat length) {
    GLfloat orig_length = sqrtf(
            powf(vector.x, 2) +
            powf(vector.y, 2) +
            powf(vector.z, 2)
    );
    vector.operator*=(length / orig_length);
}

void put_into_array(GLfloat *vertex_array, unsigned offset, glm::vec3 vertex) {
    offset = offset * ATTR_COUNT;

    vertex_array[offset] = vertex.x;
    vertex_array[offset + 1] = vertex.y;
    vertex_array[offset + 2] = vertex.z;
    // TODO: calculate RGB values for each vertex
    vertex_array[offset + 3] = offset / ATTR_COUNT == 3 ? 1.0f : 0.0f; // Different color for debugging
    vertex_array[offset + 4] = 1.0f;
    vertex_array[offset + 5] = 0.0f;
}

unsigned sphere_vertex_count_hint(unsigned lod) {
    // Sum of the first lod numbers
    // (see https://github.com/MATF-RG18/RG16-mozaik/wiki/Miscellaneous-code-and-geometry-explanations)
    //  return lod * (lod + 1) / 2;
    // Test data, for now:
    return 4;

}

unsigned sphere_index_count_hint(unsigned lod) {
    //return static_cast<unsigned int>(pow(3, (lod - 1) / 2));
    return 6; // Test data
}
