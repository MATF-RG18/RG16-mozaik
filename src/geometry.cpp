#include "geometry.hpp"
#include <cmath>
float determine_angle(glm::vec2 vector) {
    return vector.y >= 0 ? acosf(vector.x) : 2 * M_PIf32 - acosf(vector.x);
}

float determine_angle(glm::vec3 vector) {
    return determine_angle(glm::vec2(vector.x, vector.y));
}

glm::vec3 xy_plane_intersection(glm::vec3 &start_point, glm::vec3 &direction) {
    glm::vec3 intersect;
    intersect.z = 0; // Because the point is located on the z=0 plane.
    // Calculating the parameter of the parametrized form.
    float param = -(start_point.z / direction.z);
    // Calculating rest of the coordinates based on the parameter.
    intersect.x = start_point.x + param * direction.x;
    intersect.y = start_point.y + param * direction.y;
    return intersect;
}
