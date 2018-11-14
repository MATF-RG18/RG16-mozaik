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
#include "grid.hpp"

#define ATTR_COUNT 6

GLuint init_shaders();
static void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mods);
static void cursor_pos_callback(GLFWwindow* window, double x_pos, double y_pos);

static glm::vec3 position = glm::vec3(1.5f);
static const float speed = 0.1f;

// Look direction
static glm::vec3 look_direction = glm::vec3(-1.5f);
static float look_horiz_angle = 0.0f;
static float look_vert_angle = float(M_PI_2);
static bool snapped_initially = false;

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
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);

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
    GLfloat grid_vertices[grid_vertex_count_hint(10)];
    create_grid(grid_vertices, 10);

    // Create a sphere
    unsigned lod = 33;
    GLfloat sphere_vertices[sphere_vertex_count_hint(lod) * ATTR_COUNT];
    GLuint sphere_indices[sphere_index_count_hint(lod)];
    create_sphere(sphere_vertices, sphere_indices, 1.0f, lod);

    // Static draw because data is written once and used many times.
    glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices) + sizeof(sphere_vertices), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(grid_vertices), grid_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(grid_vertices), sizeof(sphere_vertices), sphere_vertices);

    GLuint element_buffer;
    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_indices), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(sphere_indices), sphere_indices);

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
    // The angle will be different when camera movement is implemented
    GLint uniform_model = glGetUniformLocation(shader_program, "model");
    model_trans = glm::rotate(model_trans, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model_trans));

    GLint uniform_view = glGetUniformLocation(shader_program, "view");

    glm::mat4 view_trans = glm::lookAt(
                position,                    // Eye coordinates
                position + look_direction, // Point to look at
                glm::vec3(0.0f, 0.0f, 1.0f)  // Up vector
    );
    glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(view_trans));

    // Projection transformation
    // TODO: remove hardcoded values
    glm::mat4 projection_trans = glm::perspective(
            glm::radians(60.0f), 800.0f / 600.0f, 1.0f, 10.0f);
    GLint uniform_projection = glGetUniformLocation(shader_program, "projection");
    glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection_trans));

    glEnable(GL_DEPTH_TEST);

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        view_trans = glm::lookAt(
                position,                    // Eye coordinates
                position + look_direction, // Point to look at
                glm::vec3(0.0f, 0.0f, 1.0f)  // Up vector
        );
        glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(view_trans));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Draw the grid
        glDrawArrays(GL_LINES, 0, 40);
        // Draw the sphere
        for (int i = 0; i < 4; i++) {
            model_trans = glm::rotate(model_trans, glm::radians(i * 90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model_trans));
            glDrawElementsBaseVertex(GL_TRIANGLES, sizeof(sphere_indices) / sizeof(sphere_indices[0]),
                                     GL_UNSIGNED_INT, 0, (sizeof(grid_vertices)) / (ATTR_COUNT * sizeof(GLfloat)));
        }
        model_trans = glm::rotate(model_trans, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        for (int i = 0; i < 4; i++) {
            model_trans = glm::rotate(model_trans, glm::radians(i * 90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model_trans));
            glDrawElementsBaseVertex(GL_TRIANGLES, sizeof(sphere_indices) / sizeof(sphere_indices[0]),
                                     GL_UNSIGNED_INT, 0, (sizeof(grid_vertices)) / (ATTR_COUNT * sizeof(GLfloat)));
        }

        glfwSwapBuffers(window);
        glfwWaitEvents();
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

        vec3 hsv2rgb(vec3 c);

        void main() {
            vec4 world_pos = model * vec4(position, 1.0);
            gl_Position = projection * view * world_pos;

            /* Arbitrary function to control color by angle */
            vec2 vector_proj = normalize(world_pos.xy);
            float hue = vector_proj.y;
            hue *= 0.25;
            if (vector_proj.y >= 0) {
                if (vector_proj.x >= 0) {
                    /* First quadrant */
                    /* Do nothing */
                } else {
                    /* Second quadrant */
                    hue = 0.5 - hue;
                }
            } else {
                if (vector_proj.x < 0) {
                    /* Third quadrant */
                    hue = 0.5 - hue;
                } else {
                    /* Fourth quadrant */
                    hue =  1.0 + hue;
                }
            }
            vec3 rgb = hsv2rgb(vec3(hue, 1.0, 1.0));
            Color = vec3(rgb);
        }
        /* Taken from https://github.com/hughsk/glsl-hsv2rgb/blob/master/index.glsl */
        vec3 hsv2rgb(vec3 c) {
            vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
            vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
            return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mods) {
    // TODO: to avoid initial pause between press and repeat, listen for press
    // and release events, and move in between.
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        position.x -= speed;
    }
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        position.x += speed;
    }
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        position.y -= speed;
    }
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        position.y += speed;
    }
}

void cursor_pos_callback(GLFWwindow *window, double x_pos, double y_pos) {
    if (!snapped_initially) {
        snapped_initially = true;
    } else {
        look_horiz_angle += (800/2 - x_pos) * 0.001;
        look_vert_angle += (600/2 - y_pos) * 0.001;

        // Parametrization of a sphere by angles and radius (radius = 1)
        look_direction = glm::vec3(
                sin(look_vert_angle) * cos(look_horiz_angle),
                sin(look_vert_angle) * sin(look_horiz_angle),
                cos(look_vert_angle)
        );
    }

    glfwSetCursorPos(window, 800/2, 600/2);

    printf("look direction: %f %f %f\n", look_direction.x, look_direction.y, look_direction.z);
}
#pragma clang diagnostic pop