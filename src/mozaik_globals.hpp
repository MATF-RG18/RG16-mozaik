#ifndef RG16_MOZAIK_MOZAIK_GLOBALS_HPP
#define RG16_MOZAIK_MOZAIK_GLOBALS_HPP

#define ATTR_COUNT 6

#include <glm/glm.hpp>

/** Determines angle between given vector and (1.0, 0.0)
 *
 * @param vector
 * Normalized vector.
 * @return
 * Angle in radians.
 */
float determine_angle(glm::vec2 vector);

float determine_angle(glm::vec3 vector);

#endif //RG16_MOZAIK_MOZAIK_GLOBALS_HPP
