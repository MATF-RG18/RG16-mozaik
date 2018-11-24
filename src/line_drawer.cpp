#include <cstdio>
#include <GL/glew.h>
#include <numeric>

#include "line_drawer.hpp"
#include "mozaik_globals.hpp"

void LineDrawer::add_to_list(glm::vec3 node) {
    node_list.push_back(node);
    buffer_vertices();
}

// Vertex buffer contents are overwritten each time (inefficient), will be improved in the future.
void LineDrawer::buffer_vertices() {
    GLfloat vertex_buffer[node_list.size() * ATTR_COUNT];
    int offset = 0;
    for (auto vertex : node_list) {
        vertex_buffer[offset] = vertex.x;
        vertex_buffer[offset + 1] = vertex.y;
        vertex_buffer[offset + 2] = vertex.z;
        vertex_buffer[offset + 3] = 1.0f;
        vertex_buffer[offset + 4] = 1.0f;
        vertex_buffer[offset + 5] = 1.0f;

        offset += ATTR_COUNT;
    }

    glBufferSubData(GL_ARRAY_BUFFER, vertex_buffer_offset, sizeof(vertex_buffer), vertex_buffer);
}

GLsizei LineDrawer::num_of_elements() {
    return static_cast<GLsizei>(node_list.size());
}