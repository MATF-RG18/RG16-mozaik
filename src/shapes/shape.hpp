#ifndef RG16_MOZAIK_SHAPE_HPP
#define RG16_MOZAIK_SHAPE_HPP


#include <GL/glew.h>
#include <glm/mat4x4.hpp>

class Shape {
private:
    GLfloat* vertex_buffer;
    GLfloat* element_buffer;
    GLfloat vertex_buffer_offset;
    GLfloat element_buffer_offset;
    glm::mat4 model_matrix;
public:
    /** Populate the buffers with data
     *
     * Populates the vertex buffer and (if applicable) element buffer.
     *
     * This function should update offsets after use.
     *
     * @param vertex_buffer
     * @param element_buffer
     * @param vertex_buffer_offset
     * @param element_buffer_offset
     */
    virtual void populate_buffer(
            GLuint vertex_buffer,
            GLuint element_buffer,
            GLfloat &vertex_buffer_offset,
            GLfloat &element_buffer_offset) = 0;

    /** Draws the shape.
     *
     *  This function is expected to be called in the main loop of the graphics
     *  renderer, and should use OpenGL functions to do so.
     */
    virtual void draw_shape() = 0;
};

#endif //RG16_MOZAIK_SHAPE_HPP
