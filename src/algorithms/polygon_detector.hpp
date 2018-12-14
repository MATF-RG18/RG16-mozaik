#ifndef RG16_MOZAIK_POLYGON_DETECTOR_HPP
#define RG16_MOZAIK_POLYGON_DETECTOR_HPP

#include <vector>
#include <glm/vec2.hpp>

class PolygonDetector {

private:
    class LineSegment {
        glm::vec2 start_point;
        glm::vec2 end_point;
    };

    std::vector<LineSegment*> line_segments;

public:

};

#endif //RG16_MOZAIK_POLYGON_DETECTOR_HPP
