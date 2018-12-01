#ifndef RG16_MOZAIK_GRID_COMPAT_HPP
#define RG16_MOZAIK_GRID_COMPAT_HPP

#include <GL/glew.h>
#include <glm/vec2.hpp>

/** Create a grid by creating pairs of vertices
 *
 *  The function fills vertex_buffer with pairs of vertices meant to be drawn by GL_LINES mode.
 *  The grid is drawn in the bottom_left to top_right vertices in the XY-plane.
 *
 * @param vertex_buffer
 * Initialized buffer. Use the hint function to determine size.
 * @param num_of_lines
 * Number of lines in each direction.
 * @param bottom_left
 * Bottom left vertex to draw from.
 * @param top_right
 * Top right vertex to draw to.
 */
void create_grid(GLfloat vertex_buffer[], unsigned num_of_lines, glm::vec2 bottom_left, glm::vec2 top_right);

unsigned grid_vertex_count_hint(unsigned lines);
#endif //RG16_MOZAIK_GRID_COMPAT_HPP
