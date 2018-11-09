#ifndef RG16_MOZAIK_COLOR_SPHERE_HPP
#define RG16_MOZAIK_COLOR_SPHERE_HPP

#include <GL/glew.h>
#include <glm/vec3.hpp>

/** Create a sphere approximation based on octahedron "inflation"
 *
 * The sphere is generated by dividing octahedron sides into smaller
 * triangles, and then normalizing their distance from the center.
 *
 * @param vertex_array
 * The array of resulting vertices, which should be manually deallocated.
 * @param va_size
 * Size of the resulting vertex array.
 * @param element_array
 * The array of resulting triangle elements, which should be manually deallocated.
 * @param ea_size
 * Size of the resulting element array.
 * @param center
 * Center of the resulting sphere.
 * @param radius
 * Radius of the resulting sphere.
 * @param lod
 * Level of detail - determines the number of vertices per side of the sphere.
 */
void create_sphere(
        GLfloat* vertex_array,
        GLsizeiptr &va_size,
        GLuint* element_array,
        GLsizeiptr &ea_size,
        glm::vec3 center,
        GLfloat radius,
        unsigned lod);
#endif //RG16_MOZAIK_COLOR_SPHERE_HPP
