#ifndef RG16_MOZAIK_GRID_HPP
#define RG16_MOZAIK_GRID_HPP

#include <GL/glew.h>
#include <glm/vec3.hpp>

/** Create a grid by creating pairs of vertices
 *
 *  The function fills vertex_buffer with pairs of vertices meant to be drawn by GL_LINES mode.
 *  The grid is drawn in the (-1,-1) to (1, 1) space.
 *
 * @param vertex_buffer
 * Initialized buffer. Use the hint function to determine size.
 * @param num_of_lines
 * Number of lines in each direction.
 */
void create_grid(GLfloat vertex_buffer[], unsigned num_of_lines);

unsigned grid_vertex_count_hint(unsigned lines);
#endif //RG16_MOZAIK_GRID_HPP
