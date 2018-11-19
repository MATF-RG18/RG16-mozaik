#ifndef RG16_MOZAIK_GEOMETRY_HPP
#define RG16_MOZAIK_GEOMETRY_HPP

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

/** Determines angle between given vector and (1.0, 0.0)
 *
 * @param vector
 * Normalized vector.
 * @return
 * Angle in radians.
 */
float determine_angle(glm::vec2 vector);

float determine_angle(glm::vec3 vector);

/** Determine an intersection of a line and the z=0 plane
 *
 * This function is substituting the glm::intersectRaySphere function, from
 * the GTX extensions, which is missing. Additionally, this function could
 * potentially be more efficient because it calculates only the specific case
 * of z=0 plane intersection.
 *
 * This function implicitly handles the case of no intersection by returning
 * "inf" values. However, in many cases OpenGL has approximation errors, so
 * the cases where the plane and the vector are truly parallel are rare.
 *
 * @param start_point
 * The point on the line.
 * @param direction
 * The direction vector of the line (not necessarily normalized).
 * @return
 * Intersection point between the line and the plane.
 */
glm::vec3 xy_plane_intersection(glm::vec3& start_point, glm::vec3& direction);

#endif //RG16_MOZAIK_GEOMETRY_HPP
