#include "color_sphere.hpp"
#include <math.h>
#include <cstdio>


GLfloat* create_sphere(glm::vec3 center, GLfloat diameter, unsigned precision_level) {

    // Testing
    glm::vec3 center_test = glm::vec3(0.0f);
    glm::vec3 vertex_test = glm::vec3(1.0f, 1.0f, 1.0f);

    normalize(center_test, vertex_test, 2.0f);

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
