#ifndef RG16_MOZAIK_INTERSECTION_DETECTION_HPP
#define RG16_MOZAIK_INTERSECTION_DETECTION_HPP

#include <vector>
#include <glm/vec2.hpp>

/** Detect intersections of line segment
 *
 * Uses a brute-force algorithm to determine intersections between lines.
 *
 */
std::vector<glm::vec2> detect_intersections(std::vector<glm::vec2> &line_segments);

#endif //RG16_MOZAIK_INTERSECTION_DETECTION_HPP