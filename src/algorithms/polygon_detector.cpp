#include "polygon_detector.hpp"

void PolygonDetector::add_line_segment(glm::vec2 point1, glm::vec2 point2) {
    line_segments.push_back(new LineSegment(point1, point2));
}