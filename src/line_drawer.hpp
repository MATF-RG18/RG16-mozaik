#ifndef RG16_MOZAIK_LINE_DRAWER_HPP
#define RG16_MOZAIK_LINE_DRAWER_HPP

#include <list>
#include <iostream>
#include <glm/vec3.hpp>

class LineDrawer {
private:
    std::list<glm::vec3> node_list;
public:
    void add_to_list(glm::vec3 node);
    void print_list();
};

#endif //RG16_MOZAIK_LINE_DRAWER_HPP
