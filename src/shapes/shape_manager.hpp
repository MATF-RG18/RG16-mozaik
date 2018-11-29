#ifndef RG16_MOZAIK_SHAPE_MANAGER_HPP
#define RG16_MOZAIK_SHAPE_MANAGER_HPP

#include <list>
#include "shape.hpp"

/** This class stores the references to all the shapes that will be included
 * in the scene, takes care of populating the buffer, and rendering the shapes
 * onto the scene.
 *
 */
class ShapeManager {
private:
    GLuint vertex_buffer;
    GLuint element_buffer;
    GLuint uniform_model;
    GLsizei vertex_buffer_size = 0;
    GLsizei element_buffer_size = 0;
    std::list<Shape*> shape_list;
public:
    /** Adds the shape to the list of shapes to be buffered and drawn. */
    void subscribe_shape(Shape* shape);
    /** Allocates the appropriate space for the shapes and populates the buffer with their data. */
    void populate_buffer();
    /** Renders the shapes */
    void render();
};


#endif //RG16_MOZAIK_SHAPE_MANAGER_HPP
