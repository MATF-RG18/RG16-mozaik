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

void create_sphere(
        GLfloat* &vertex_array,
        GLsizei &va_size,
        GLuint* &element_array,
        GLsizei &ea_size,
        glm::vec3 center,
        GLfloat radius,
        unsigned lod) {

    // Sum of the first lod numbers
    // (see https://github.com/MATF-RG18/RG16-mozaik/wiki/Miscellaneous-code-and-geometry-explanations)
    va_size = lod * (lod + 1) / 2;
    vertex_array = new GLfloat[va_size * ATTR_COUNT];

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
    va_size = 4 * ATTR_COUNT;

    element_array = new GLuint[3];
    element_array[0] = 0;
    element_array[1] = 1;
    element_array[2] = 2;
    element_array[3] = 0;
    element_array[4] = 1;
    element_array[5] = 3;
    ea_size = 6;
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
    vertex_array[offset + 3] = offset == 3 ? 1.0f : 0.0f;
    vertex_array[offset + 4] = 1.0f;
    vertex_array[offset + 5] = 0.0f;

}
