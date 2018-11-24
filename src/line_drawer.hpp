#ifndef RG16_MOZAIK_LINE_DRAWER_HPP
#define RG16_MOZAIK_LINE_DRAWER_HPP

#include <list>
#include <iostream>
#include <glm/vec3.hpp>
#include <GL/glew.h>

class LineDrawer {
private:
    std::list<glm::vec3> node_list;
    void buffer_vertices();
public:
    int vertex_buffer_offset;

    void add_to_list(glm::vec3 node);
    GLsizei num_of_elements();
};

#endif //RG16_MOZAIK_LINE_DRAWER_HPP
