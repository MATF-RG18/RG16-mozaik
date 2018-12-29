#include <glm/ext.hpp>
#include "color_sphere.hpp"
#include "../mozaik_globals.hpp"
#include "../geometry.hpp"

ColorSphere::ColorSphere(unsigned lod, GLfloat radius, glm::mat4 model_trans)
    : Shape(model_trans, GL_TRIANGLES, SPHERE) {

    // Number of vertices per one octahedron side * 8 sides of an octahedron
    vertex_data = new GLfloat[triangle_vertex_data_size_hint(lod) * 8 * ATTR_COUNT];
    vertex_data_size = triangle_vertex_data_size_hint(lod) * 8 * ATTR_COUNT * sizeof(GLfloat);
    // TODO: visual explanation
    element_data = new GLint[(int) pow(lod - 1, 2) * 3 * 8];
    element_data_size = static_cast<GLsizei>((pow(lod - 1, 2) * 3 * 8) * sizeof(GLint));

    // The point from which all other points will be translated
    glm::vec3 origin = glm::vec3(1.0f, 0.0f, 0.0f);

    // Base vectors which will be multiplied and added to the origin vector
    glm::vec3 base1 = glm::vec3(-1.0f, 1.0f, 0.0f);
    glm::vec3 base2 = glm::vec3(-1.0f, 0.0f, 1.0f);

    // Normalize to match length of a single distance between vertices
    normalize(base1, static_cast<GLfloat>(powf(2.0f, 1.0f / 2) / (lod - 1)));
    normalize(base2, static_cast<GLfloat>(powf(2.0f, 1.0f / 2) / (lod - 1)));

    glm::mat4 rotation_matrix = glm::mat4(1.0f);
    glm::vec3 z_axis = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 x_axis = glm::vec3(1.0f, 0.0f, 0.0f);

    unsigned triangle_size = triangle_vertex_data_size_hint(lod);

    for (int hemisphere = 0; hemisphere < 2; hemisphere++) {
        for (int side = 0; side < 4; side++) {
            // Vertex buffer entry
            for (int j = 0; j < lod; j++) {
                for (int i = 0; i < lod - j; ++i) {
                    glm::vec3 vector = origin + multiply(base1, i) + multiply(base2, j);
                    normalize(vector, radius);
                    glm::vec4 rot_vector = rotation_matrix * glm::vec4(vector, 1.0);
                    put_into_vertex_data(glm::vec3(rot_vector.x, rot_vector.y, rot_vector.z));
                }
            }
            rotation_matrix = glm::rotate(rotation_matrix, M_PI_2f32, z_axis);

            // Index buffer entry
            unsigned face_offset = (hemisphere * 4 + side) * triangle_size;
            unsigned offset = 0;
            for (int j = 0; j < lod; j++) {
                for (unsigned i = 0; i < lod - j - 1; ++i) {
                    // Upward-facing triangles
                    put_into_index_array(face_offset + offset + i,
                                         face_offset + offset + i + 1,
                                         face_offset + offset + lod - j + i);
                    if (i < lod - j - 2) {
                        // Downward-facing triangles
                        put_into_index_array(face_offset + offset + lod - j + i,
                                             face_offset + offset + lod - j + i + 1,
                                             face_offset + offset + i + 1);
                    }
                }
                offset += (lod - j);
            }
        }
        rotation_matrix = glm::rotate(rotation_matrix, M_PIf32, x_axis);
    }


}

void ColorSphere::normalize(glm::vec3 &vector, GLfloat length) {
    GLfloat orig_length = sqrtf(
            powf(vector.x, 2) +
            powf(vector.y, 2) +
            powf(vector.z, 2)
    );
    vector.operator*=(length / orig_length);
}

glm::vec3 ColorSphere::multiply(glm::vec3 vector, GLfloat factor) {
    vector.x *= factor;
    vector.y *= factor;
    vector.z *= factor;
    return vector;
}

void ColorSphere::put_into_vertex_data(glm::vec3 vertex) {
    static unsigned offset;

    vertex_data[offset] = vertex.x;
    vertex_data[offset + 1] = vertex.y;
    vertex_data[offset + 2] = vertex.z;

    glm::vec2 vector_proj = glm::normalize(glm::vec2(vertex.x, vertex.y));
    float hue = vector_proj.y;
    hue *= 0.25f;
    if (vector_proj.y >= 0) {
        if (vector_proj.x >= 0) {
            /* First quadrant */
            /* Do nothing */
        } else {
            /* Second quadrant */
            hue = 0.5f - hue;
        }
    } else {
        if (vector_proj.x < 0) {
            /* Third quadrant */
            hue = 0.5f - hue;
        } else {
            /* Fourth quadrant */
            hue =  1.0f + hue;
        }
    }

    glm::vec3 result_color = hsv2rgb(hue, 1.0f, 1.0f);

    vertex_data[offset + 3] = result_color.r;
    vertex_data[offset + 4] = result_color.g;
    vertex_data[offset + 5] = result_color.b;

    offset += ATTR_COUNT;
}

void ColorSphere::put_into_index_array(GLuint first, GLuint second, GLuint third) {
    static unsigned offset;

    element_data[offset] = first;
    element_data[offset + 1] = second;
    element_data[offset + 2] = third;

    offset += 3;
}

unsigned ColorSphere::triangle_vertex_data_size_hint(unsigned lod) {
    // Sum of the first lod numbers
    // (see https://github.com/MATF-RG18/RG16-mozaik/wiki/Miscellaneous-code-and-geometry-explanations)
    return (lod * (lod + 1) / 2);
}

/* Taken from https://github.com/hughsk/glsl-hsv2rgb/blob/master/index.glsl */
glm::vec3 ColorSphere::hsv2rgb(float h, float s, float b) {
    glm::vec4 K = glm::vec4(1.0f, 2.0f / 3.0f, 1.0f / 3.0f, 3.0f);
    glm::vec3 p = glm::abs(glm::fract(glm::vec3(h) + glm::vec3(K.x, K.y, K.z)) * 6.0f - glm::vec3(K.w));
    return b * glm::mix(glm::vec3(K.x), glm::clamp(p - glm::vec3(K.x), 0.0f, 1.0f), s);
}
