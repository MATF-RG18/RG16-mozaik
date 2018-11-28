#ifndef RG16_MOZAIK_SHAPE_HPP
#define RG16_MOZAIK_SHAPE_HPP


#include <GL/glew.h>
#include <glm/mat4x4.hpp>

class Shape {
private:
    GLfloat* vertex_buffer;
    GLfloat* element_buffer;
    glm::mat4 model_matrix;
};

#endif //RG16_MOZAIK_SHAPE_HPP
