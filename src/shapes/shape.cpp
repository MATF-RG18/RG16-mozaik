#include "shape.hpp"

Shape::Shape(const glm::mat4 &model_matrix, GLenum draw_mode, ShapeType shape_type)
        : model_matrix(model_matrix), draw_mode(draw_mode), shape_type(shape_type) {}
