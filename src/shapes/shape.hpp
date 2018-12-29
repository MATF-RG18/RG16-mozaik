#ifndef RG16_MOZAIK_SHAPE_HPP
#define RG16_MOZAIK_SHAPE_HPP


#include <GL/glew.h>
#include <glm/mat4x4.hpp>

enum ShapeType {
    GRID, TILES, SPHERE, CROSSHAIR
};

class Shape {
public:
    GLfloat* vertex_data;
    GLint* element_data;
    GLsizei vertex_data_size;
    // -1 if the shape doesn't use the element buffer.
    GLsizei element_data_size = -1;
    GLintptr vertex_buffer_offset;
    GLintptr element_buffer_offset = -1;
    glm::mat4 model_matrix;
    GLenum draw_mode;
    ShapeType shape_type;

    Shape(const glm::mat4 &model_matrix, GLenum draw_mode, ShapeType shape_type);
};

#endif //RG16_MOZAIK_SHAPE_HPP
