#ifndef RG16_MOZAIK_POLYGON_DETECTOR_HPP
#define RG16_MOZAIK_POLYGON_DETECTOR_HPP

#include <vector>
#include <glm/vec2.hpp>

class PolygonDetector {

private:
    class LineSegment {
        glm::vec2 start_point;
        glm::vec2 end_point;

    public:
        LineSegment(const glm::vec2 &start_point, const glm::vec2 &end_point) :
            start_point(start_point), end_point(end_point) {}
    };

    std::vector<LineSegment*> line_segments;

public:
    void add_line_segment(glm::vec2 point1, glm::vec2 point2);
};

#endif //RG16_MOZAIK_POLYGON_DETECTOR_HPP
