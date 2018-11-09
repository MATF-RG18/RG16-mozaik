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
static void normalize(glm::vec3 center, glm::vec3 &orig, GLfloat length);

GLfloat* create_sphere(glm::vec3 center, GLfloat diameter, unsigned lod) {

    // Testing
    glm::vec3 vertex_test = glm::vec3(1.0f, 1.0f, 1.0f);

    normalize(center, vertex_test, diameter);

    printf("%f %f %f\n", vertex_test.x, vertex_test.y, vertex_test.z);

    return nullptr;
}

void normalize(glm::vec3 center, glm::vec3 &orig, GLfloat length) {
    GLfloat distance = sqrtf(
            powf(orig.x - center.x, 2) +
            powf(orig.y - center.y, 2) +
            powf(orig.z - center.z, 2)
    );
    orig.x = orig.x * length / distance;
    orig.y = orig.y * length / distance;
    orig.z = orig.z * length / distance;
}
