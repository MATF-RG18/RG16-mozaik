#include "intersection_detection.hpp"
#include <cstdio>

enum OrientationRestriction {
    CLOCKWISE, COUNTERCLOCKWISE, NONE
};

struct IntersectionInfo {
    glm::vec2 intersection_point;
    // Intersecting segment id
    int segment_id;
    // Parameter of the point on the intersecting segment
    float param;
};

/* Solve a system of linear equations:
 * a1x + a2y = a3
 * b1x + b2y = b3
 */
static std::pair<float, float> cramers_rule(float a1, float a2, float a3, float b1, float b2, float b3, bool &unique_solution);

std::vector<glm::vec2> detect_intersections(std::vector<glm::vec2> &line_segments) {
    //Testing
    // Line segments A-B and C-D
    glm::vec2 pointA = glm::vec2(0.0f, 0.0f);
    glm::vec2 pointB = glm::vec2(2.0f, 2.0f);
    glm::vec2 pointC = glm::vec2(0.0f, 2.0f);
    glm::vec2 pointD = glm::vec2(2.0f, 0.0f);
    
    bool unique_solution;
    // t*(xB - xA) + p*(xC - xD) = xC - xA
    // t*(yB - yA) + p*(yC - yD) = yC - yA
    std::pair<float, float> intersection_params = cramers_rule(
        pointB.x - pointA.x, pointC.x - pointD.x, pointC.x - pointA.x,
        pointB.y - pointA.y, pointC.y - pointD.y, pointC.y - pointA.y,
        unique_solution
    );

    //Expected intersection at (1, 1)
    if (unique_solution) {
        if (intersection_params.first >= 0 && intersection_params.first <= 1
            && intersection_params.second >= 0 && intersection_params.second <= 1) {
            // intersection = A + t(B-A)
            glm::vec2 intersection = pointA + intersection_params.first * (pointB - pointA);
            printf("Intersection at: (%f, %f)\n", intersection.x, intersection.y);
        } else {
            printf("Unique solution, parameters out of bounds: %f, %f\n", intersection_params.first,
                   intersection_params.second);
        }
    } else {
        printf("Not a unique solution\n");
    }

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