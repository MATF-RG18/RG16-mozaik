#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define GLEW_STATIC
#pragma clang diagnostic pop

#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "color_sphere.hpp"

#define ATTR_COUNT 6

GLuint init_shaders();

int main() {
    //Initialize window framework
    glfwInit();
    // OpenGL version 3.2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    // Using core-profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // Resizing not implemented yet
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Windowed mode
    GLFWwindow *window = glfwCreateWindow(800, 600, "Mozaik", nullptr, nullptr);
    // Context must be made current so OpenGL calls can take effect
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Initialize buffers
    GLuint vertex_array_buffer;
    glGenVertexArrays(1, &vertex_array_buffer);
    glBindVertexArray(vertex_array_buffer);

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    // Create a grid
    // TODO: wrap into a parametrized function

    // 20 lines * 2 vertices each * attribute count
    float grid_vertices[20 * 2 * ATTR_COUNT];

    for (int i = 0; i < 10; i++) {
        // 4 vertices * attribute count
        int offset = i * ATTR_COUNT * 4;

        // Start point
        grid_vertices[offset] = -1.0f + i * 0.2f;        // X
        grid_vertices[offset + 1] = -1.0f;               // Y
        grid_vertices[offset + 2] = 0.0f;                // Z
        grid_vertices[offset + 3] = 1.0f;                // R
        grid_vertices[offset + 4] = 1.0f;                // G
        grid_vertices[offset + 5] = 1.0f;                // B

        // End point
        grid_vertices[offset + 6] = -1.0f + i * 0.2f;    // X
        grid_vertices[offset + 7] = 1.0f;                // Y
        grid_vertices[offset + 8] = 0.0f;                // Z
        grid_vertices[offset + 9] = 1.0f;                // R
        grid_vertices[offset + 10] = 1.0f;               // G
        grid_vertices[offset + 11] = 1.0f;               // B

        // Start point
        grid_vertices[offset + 12] = -1.0f;              // X
        grid_vertices[offset + 13] = -1.0f + i * 0.2f;   // Y
        grid_vertices[offset + 14] = 0.0f;               // Z
        grid_vertices[offset + 15] = 1.0f;               // R
        grid_vertices[offset + 16] = 1.0f;               // G
        grid_vertices[offset + 17] = 1.0f;               // B

        // End point
        grid_vertices[offset + 18] = 1.0f;               // X
        grid_vertices[offset + 19] = -1.0f + i * 0.2f;   // Y
        grid_vertices[offset + 20] = 0.0f;               // Z
        grid_vertices[offset + 21] = 1.0f;               // R
        grid_vertices[offset + 22] = 1.0f;               // G
        grid_vertices[offset + 23] = 1.0f;               // B
    }

    GLfloat* sphere_vertices = create_sphere(glm::vec3(0.0f), 2.0f, 0);

    // Fan-like triangle organization, that will evolve into a sphere approximation
    const unsigned fan_count = 50; // Fan side count
    GLfloat triangle_vertices[(fan_count + 1) * ATTR_COUNT];

    // Fan hub
    triangle_vertices[0] = 0.0f; //X
    triangle_vertices[1] = 0.0f; //Y
    triangle_vertices[2] = 0.0f; //Z
    triangle_vertices[3] = 1.0f; //R
    triangle_vertices[4] = 1.0f; //G
    triangle_vertices[5] = 1.0f; //B

    for (int i = 0; i < fan_count; i++) {
        // attr count for the fan hub, and attr count for the each consequent vertex
        int offset = ATTR_COUNT + i * ATTR_COUNT;
        triangle_vertices[offset] =     static_cast<GLfloat>(cos(2 * M_PI * i / fan_count)); // X
        triangle_vertices[offset + 1] = static_cast<GLfloat>(sin(2 * M_PI * i / fan_count)); // Y
        triangle_vertices[offset + 2] = 0.0f;                                                // Z
        // Arbitrary continuous functions to map i values to [0, 1]
        triangle_vertices[offset + 3] = static_cast<GLfloat>(sin(0.5f * M_PI * i / fan_count)); // R
        triangle_vertices[offset + 4] = 0.0f;                                                   // G
        triangle_vertices[offset + 5] = static_cast<GLfloat>(cos(0.5f * M_PI * i / fan_count)); // B
    }

    // Static draw because data is written once and used many times.
    glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices) + sizeof(triangle_vertices), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(grid_vertices), grid_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(grid_vertices), sizeof(triangle_vertices), triangle_vertices);

    GLuint triangle_elements[fan_count * ATTR_COUNT];

    for (unsigned i = 0; i < fan_count; i++) {
        int offset = i * ATTR_COUNT;
        triangle_elements[offset] = 0; // One element is always the hub
        triangle_elements[offset + 1] = i + 1;
        triangle_elements[offset + 2] = i + 2;
    }

    // The last vertex should connect to the first vertex
    triangle_elements[fan_count * ATTR_COUNT - 3] = 0;
    triangle_elements[fan_count * ATTR_COUNT - 2] = fan_count;
    triangle_elements[fan_count * ATTR_COUNT - 1] = 1;

    GLuint element_buffer;
    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            sizeof(triangle_elements), triangle_elements, GL_STATIC_DRAW);

    // Initialize shaders
    GLuint shader_program = init_shaders();

    // Make a connection between vertex data and attributes
    GLuint position_attribute = static_cast<GLuint>(glGetAttribLocation(shader_program, "position"));
    glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, ATTR_COUNT * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(position_attribute);

    GLuint color_attribute = static_cast<GLuint>(glGetAttribLocation(shader_program, "color"));
    glVertexAttribPointer(color_attribute, 3,
            GL_FLOAT, GL_FALSE, ATTR_COUNT * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(color_attribute);

    // Matrix transformations

    // Model transformation (just a sample one at the moment)
    glm::mat4 model_trans = glm::mat4(1.0f);
    model_trans = glm::rotate(model_trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    GLint uniform_model = glGetUniformLocation(shader_program, "model");
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model_trans));

    // View transformation (camera movement not implemented)
    glm::mat4 view_trans = glm::lookAt(
                glm::vec3(1.5f, 1.5f, 1.5f), // Eye coordinates
                glm::vec3(0.0f, 0.0f, 0.0f), // Point to look at
                glm::vec3(0.0f, 0.0f, 1.0f)  // Up vector
    );
    GLint uniform_view = glGetUniformLocation(shader_program, "view");
    glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(view_trans));

    // Projection transformation
    // TODO: remove hardcoded values
    glm::mat4 projection_trans = glm::perspective(
            glm::radians(60.0f), 800.0f / 600.0f, 1.0f, 10.0f);
    GLint uniform_projection = glGetUniformLocation(shader_program, "projection");
    glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection_trans));

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Draw the grid
        glDrawArrays(GL_LINES, 0, 40);
        // Draw the triangles
        glDrawElementsBaseVertex(GL_TRIANGLES, sizeof(triangle_elements), GL_UNSIGNED_INT, 0, 40);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return glGetError();
}

GLuint init_shaders() {
    // TODO: move to a file in the future
    const char* vertex_shader_source = R"glsl(
        #version 150 core

        in vec3 position;
        in vec3 color;
        /* flat : the color will be sourced from the provoking vertex. */
        flat out vec3 Color;

        /* transformation matrices */
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            /* Fourth coordinate is related to clipping and should default to 1.0 */
            gl_Position = projection * view * model * vec4(position, 1.0);
            Color = color;
        }
    )glsl";

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);

    // Check whether the shader compiled successfully
    GLint status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[4096];
        glGetShaderInfoLog(vertex_shader, 4096, nullptr, buffer);
        printf("Vertex buffer compilation failed:\n%s", buffer);
    }

    // TODO: move to a file in the future
    const char* fragment_shader_source = R"glsl(
        #version 150 core

        out vec4 out_color;

        flat in vec3 Color;

        void main() {
            out_color = vec4(Color, 1.0);
        }
    )glsl";

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);

    // Check whether the shader compiled successfully (using status from before)
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[4096];
        glGetShaderInfoLog(fragment_shader, 4096, nullptr, buffer);
        printf("Fragment buffer compilation failed:\n%s", buffer);
    }

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    // Fragment shader can output to multiple buffers, so specifying buffer locations is
    // necessary. However, 0 is used by default, so the next statement is kept for
    // clarity only.
    glBindFragDataLocation(shader_program, 0, "out_color");

    glLinkProgram(shader_program);
    glUseProgram(shader_program);

    return shader_program;
}