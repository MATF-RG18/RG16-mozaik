#include "mozaik_globals.hpp"


float determine_angle(glm::vec2 vector) {
    return vector.y >= 0 ? acosf(vector.x) : 2 * M_PIf32 - acosf(vector.x);
}

float determine_angle(glm::vec3 vector) {
    return determine_angle(glm::vec2(vector.x, vector.y));
}
