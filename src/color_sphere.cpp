#include "color_sphere.hpp"
#include <math.h>
#include <cstdio>

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

void create_sphere(
        GLfloat* &vertex_array,
        GLsizeiptr &va_size,
        GLuint* &element_array,
        GLsizeiptr &ea_size,
        glm::vec3 center,
        GLfloat radius,
        unsigned lod) {

    // Sum of the first lod numbers
    // (see https://github.com/MATF-RG18/RG16-mozaik/wiki/Miscellaneous-code-and-geometry-explanations)
    va_size = lod * (lod + 1) / 2;
    vertex_array = new GLfloat[va_size];

    // The point from which all other points will be translated
    glm::vec3 origin = glm::vec3(1.0f, -1.0f, 0.0f);

    glm::vec3 base1 = glm::vec3(0.0f, 1.0f, 0.0f);
    // Normalize to match length of a single distance between vertices
    normalize(base1, 1.0f / lod);

    glm::vec3 base2 = glm::vec3(-1.0f, -1.0f, 1.0f);
    // Normalize to match length of a single distance between vertices
    normalize(base2, 1.0f / lod);
}

void normalize(glm::vec3 &vector, GLfloat length) {
    GLfloat orig_length = sqrtf(
            powf(vector.x, 2) +
            powf(vector.y, 2) +
            powf(vector.z, 2)
    );
    vector.operator*=(length / orig_length);
}
