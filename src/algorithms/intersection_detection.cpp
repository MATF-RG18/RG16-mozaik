#include "intersection_detection.hpp"

/* Solve a system of linear equations:
 * a1x + a2y = a3
 * b1x + b2y = b3
 */
static std::pair<float, float> cramers_rule(float a1, float a2, float a3, float b1, float b2, float b3, bool &unique_solution);

std::vector<glm::vec2> detect_intersections(std::vector<glm::vec2> &line_segments) {
    return std::vector<glm::vec2>();
}

std::pair<float, float> cramers_rule(float a1, float a2, float a3, float b1, float b2, float b3, bool &unique_solution) {
    float determinant = a1 * b2 - a2 * b1;
    if (determinant != 0) {
        unique_solution = true;
        float x = (a3 * b2 - a2 * b3) / determinant;
        float y = (a1 * b3 - a3 * b1) / determinant;
        return std::pair<float, float>(x, y);
    } else {
        unique_solution = false;
        return std::pair<float, float>();
    }
}