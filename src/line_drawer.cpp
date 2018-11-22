#include <cstdio>
#include "line_drawer.hpp"

void LineDrawer::add_to_list(glm::vec3 node) {
    node_list.push_back(node);
}

// Testing
void LineDrawer::print_list() {
    for (auto node : node_list) {
        printf("(%f %f %f)\n", node.x, node.y, node.z);
    }
    printf("--\n");
}
