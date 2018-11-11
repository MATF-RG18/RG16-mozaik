#include "color_sphere.hpp"
#include <math.h>
#include <cstdio>
#include <random>
#include <functional>

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

static glm::vec3 multiply(glm::vec3 vector, GLfloat factor);

static void put_into_vertex_array(GLfloat vertex_array[], glm::vec3 vertex);

static void put_into_index_array(GLuint index_array[], GLuint first, GLuint second, GLuint third);

void create_sphere(GLfloat vertex_array[], GLuint index_array[], GLfloat radius, unsigned lod) {

    // The point from which all other points will be translated
    glm::vec3 origin = glm::vec3(1.0f, 0.0f, 0.0f);

    glm::vec3 base1 = glm::vec3(-1.0f, 1.0f, 0.0f);
    // Normalize to match length of a single distance between vertices
    normalize(base1, static_cast<GLfloat>(powf(2.0f, 1.0f / 2.0f) / (lod - 1)));

    glm::vec3 base2 = glm::vec3(-1.0f, 0.0f, 1.0f);
    // Normalize to match length of a single distance between vertices
    normalize(base2, static_cast<GLfloat>(powf(2.0f, 1.0f / 2.0f) / (lod - 1)));

    for (int j = 0; j < lod; j++) {
        for (int i = 0; i < lod - j; ++i) {
            glm::vec3 vector = origin + multiply(base1, i) + multiply(base2, j);
            normalize(vector, radius);
            put_into_vertex_array(vertex_array, vector);
        }
    }

    int offset = 0;
    for (int j = 0; j < lod; j++) {
        for (unsigned i = 0; i < lod - j - 1; ++i) {
            put_into_index_array(index_array, i + offset, i + offset + 1, lod - j + i + offset);
            if (i < lod - j - 2) {
                put_into_index_array(index_array, lod - j + i + offset, lod - j + i + offset + 1, i + offset + 1);
            }
        }
        offset += (lod - j);
    }
}

void normalize(glm::vec3 &vector, GLfloat length) {
    GLfloat orig_length = sqrtf(
            powf(vector.x, 2) +
            powf(vector.y, 2) +
            powf(vector.z, 2)
    );
    vector.operator*=(length / orig_length);
}

glm::vec3 multiply(glm::vec3 vector, GLfloat factor) {
    vector.x *= factor;
    vector.y *= factor;
    vector.z *= factor;

    return vector;
}

void put_into_vertex_array(GLfloat vertex_array[], glm::vec3 vertex) {
    static unsigned offset;
    static std::default_random_engine generator;
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    static auto rand = std::bind(distribution, generator);

    vertex_array[offset] = vertex.x;
    vertex_array[offset + 1] = vertex.y;
    vertex_array[offset + 2] = vertex.z;
    // TODO: calculate RGB values for each vertex
    vertex_array[offset + 3] = rand();
    vertex_array[offset + 4] = rand();
    vertex_array[offset + 5] = rand();

    offset += ATTR_COUNT;
}

void put_into_index_array(GLuint index_array[], GLuint first, GLuint second, GLuint third) {
    static unsigned offset;

    index_array[offset] = first;
    index_array[offset + 1] = second;
    index_array[offset + 2] = third;

    offset += 3;
}

unsigned sphere_vertex_count_hint(unsigned lod) {
    // Sum of the first lod numbers
    // (see https://github.com/MATF-RG18/RG16-mozaik/wiki/Miscellaneous-code-and-geometry-explanations)
    return lod * (lod + 1) / 2;

}

unsigned sphere_index_count_hint(unsigned lod) {
    // TODO: visual explanation
    return static_cast<unsigned int>(pow(lod - 1, 2) * 3);
}
