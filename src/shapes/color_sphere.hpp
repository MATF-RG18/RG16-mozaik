#ifndef RG16_MOZAIK_COLOR_SPHERE_HPP
#define RG16_MOZAIK_COLOR_SPHERE_HPP

#include "shape.hpp"
/** Sphere approximation based on octahedron "inflation"
 *
 * Eight sides of octahedron are split into vertices, and those vertices have their distance
 * from the center normalized, resulting in a sphere.
 *
 * "lod" parameter defines the level of detail, and should always be a power of two + 1.
 *
 */
class ColorSphere : public Shape {
private:
    void normalize(glm::vec3 &vector, GLfloat length);

    /* Hint how many vertices will one octahedron triangle use. */
    unsigned triangle_vertex_data_size_hint(unsigned lod);

    void put_into_vertex_data(glm::vec3 vertex);

    void put_into_index_array(GLuint first, GLuint second, GLuint third);

    glm::vec3 hsv2rgb(float h, float s, float b);
public:
    ColorSphere(unsigned lod, GLfloat radius, glm::mat4 model_trans = glm::mat4(1.0f));
};


#endif //RG16_MOZAIK_COLOR_SPHERE_HPP
