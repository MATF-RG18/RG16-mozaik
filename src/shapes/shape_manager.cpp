#include <glm/ext.hpp>
#include <cstdio>
#include "shape_manager.hpp"
#include "../mozaik_globals.hpp"

void ShapeManager::subscribe_shape(Shape *shape) {
    shapes.push_back(shape);
    vertex_buffer_size += shape->vertex_data_size;
    if (shape->element_data_size != -1) {
        element_buffer_size += shape->element_data_size;
    }
}

void ShapeManager::populate_buffer() {
    // Static draw because data is written once and used many times.
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, nullptr, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_buffer_size, nullptr, GL_STATIC_DRAW);

    GLsizei current_vertex_offset = 0;
    GLsizei current_element_offset = 0;

    int objects_rendered = 0;

    for (auto shape : shapes) {
        // Buffering the grid, sphere and crosshair only, for testing purposes.
        if (objects_rendered < 3) {
            shape->vertex_buffer_offset = current_vertex_offset;
            glBufferSubData(GL_ARRAY_BUFFER, shape->vertex_buffer_offset, shape->vertex_data_size, shape->vertex_data);
            current_vertex_offset += shape->vertex_data_size;

            // If the shape uses element buffer
            if (shape->element_data_size != -1) {
                shape->element_buffer_offset = current_element_offset;
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, shape->element_buffer_offset, shape->element_data_size,
                                shape->element_data);
                current_element_offset += shape->element_data_size;
            }
            objects_rendered++;
        }
    }
}

void ShapeManager::render(ShapeType shape_type) {
    for (auto shape : shapes) {
        if (shape->shape_type == shape_type) {
            glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(shape->model_matrix));

            // If the shape uses the element buffer
            if (shape->element_data_size != -1) {
                glDrawElementsBaseVertex(
                        shape->draw_mode,
                        shape->element_data_size / sizeof(shape->element_data[0]),
                        GL_UNSIGNED_INT,
                        reinterpret_cast<const void *>(shape->element_buffer_offset),
                        static_cast<GLint>(shape->vertex_buffer_offset / (ATTR_COUNT * sizeof(GLfloat)))
                );
            } else {
                glDrawArrays(
                        shape->draw_mode,
                        static_cast<GLint>(shape->vertex_buffer_offset) / (ATTR_COUNT * sizeof(GLfloat)) ,
                        shape->vertex_data_size / (ATTR_COUNT * sizeof(shape->vertex_data[0]))
                );
            }
        }
    }
}
