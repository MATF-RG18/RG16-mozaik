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
#include "mozaik_globals.hpp"

GLuint init_shaders();
static void keyboard_callback(GLFWwindow *window, int key, int scan_code, int action, int mods);
static void cursor_pos_callback(GLFWwindow* window, double x_pos, double y_pos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static float clamp(float value, float min, float max);

// Shader program made public, so it could be accessed by callbacks.
static GLuint shader_program;

static int window_width = 800;
static int window_height = 600;

// Movement
static glm::vec3 position = glm::vec3(1.5f);
// Both vectors are needed because one is used to calculate the direction, and the other one is used to
// move by a normalized amount.
static glm::vec3 movement_vector = glm::vec3(0.0f);
static glm::vec3 normalized_movement_vector = glm::vec3(0.0f);
static float movement_speed = 1.0f;

// Look direction (initial values point to center)
static glm::vec3 look_direction = glm::vec3(-1.5f);
static float look_h_angle = -M_PI_2f32 * 1.5f;
static float look_v_angle = M_PI_2f32 * 1.5f;

int main() {
    //Initialize window framework
    glfwInit();
    // OpenGL version 3.2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    // Using core-profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Windowed mode
    GLFWwindow *window = glfwCreateWindow(window_width, window_height, "Mozaik", nullptr, nullptr);
    // Hide the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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
    unsigned num_of_lines = 100;
    GLfloat grid_vertices[grid_vertex_count_hint(num_of_lines)];
    create_grid(grid_vertices, num_of_lines, glm::vec2(-10.0f), glm::vec2(10.0f));
    glm::mat4 grid_model_trans = glm::mat4(1.0f);

    // Create a sphere
    unsigned lod = 33;
    GLfloat sphere_vertices[sphere_vertex_count_hint(lod) * ATTR_COUNT];
    GLuint sphere_indices[sphere_index_count_hint(lod)];
    create_sphere(sphere_vertices, sphere_indices, 1.0f, lod);
    glm::mat4 sphere_model_trans = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f));

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

    // Model transformation (will be set in the main loop)
    GLint uniform_model = glGetUniformLocation(shader_program, "model");

    // View transformation (will be set in the main loop)
    GLint uniform_view = glGetUniformLocation(shader_program, "view");

    // Projection transformation (also updated in framebuffer size callback)
    glm::mat4 projection_trans = glm::perspective(
            glm::radians(45.0f), (float) window_width / window_height, 1.0f, 50.0f);
    GLint uniform_projection = glGetUniformLocation(shader_program, "projection");
    glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection_trans));

    glEnable(GL_DEPTH_TEST);

    double old_time = glfwGetTime();
    // Main loop
    while (!glfwWindowShouldClose(window)) {

        double current_time = glfwGetTime();
        float delta_time = static_cast<float>(current_time - old_time);
        old_time = current_time;

        // Rotate movement vector to match look direction

        glm::vec3 ref_look_direction = look_direction;
        ref_look_direction.z = 0; // XY projection
        ref_look_direction = glm::normalize(ref_look_direction);
        float angle = determine_angle(ref_look_direction) + M_PIf32;

        glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec3 rotated_vector = rotation_matrix * glm::vec4(normalized_movement_vector, 1.0f);

        position += rotated_vector * delta_time * movement_speed;

        glm::mat4 view_trans = glm::lookAt(
                position,                    // Eye coordinates
                position + look_direction, // Point to look at
                glm::vec3(0.0f, 0.0f, 1.0f)  // Up vector
        );
        glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(view_trans));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Draw the grid
        glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(grid_model_trans));
        glDrawArrays(GL_LINES, 0, (sizeof(grid_vertices)) / (ATTR_COUNT * sizeof(GLfloat)));
        // Draw the sphere
        glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(sphere_model_trans));
        glDrawElementsBaseVertex(GL_TRIANGLES, sizeof(sphere_indices) / sizeof(sphere_indices[0]),
                                 GL_UNSIGNED_INT, 0, (sizeof(grid_vertices)) / (ATTR_COUNT * sizeof(GLfloat)));

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

    shader_program = glCreateProgram();
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
#pragma clang diagnostic ignored "-Wunused-parameter" // Ignore unused parameters for all callbacks.
void keyboard_callback(GLFWwindow *window, int key, int scan_code, int action, int mods) {

    movement_speed = mods & GLFW_MOD_SHIFT ? 3.0f : 1.0f;
    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
            movement_vector.x--;
        } else if (action == GLFW_RELEASE) {
            movement_vector.x++;
        }
    } else if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            movement_vector.x++;
        } else if (action == GLFW_RELEASE) {
            movement_vector.x--;
        }
    } else if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            movement_vector.y--;
        } else if (action == GLFW_RELEASE) {
            movement_vector.y++;
        }
    } else if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            movement_vector.y++;
        } else if (action == GLFW_RELEASE) {
            movement_vector.y--;
        }
    }

    // Only normalize if vector is different from zero-intensity vector, because it doesn't have a direction.
    if (movement_vector.x != 0 || movement_vector.y != 0) {
        normalized_movement_vector = glm::normalize(movement_vector);
    } else {
        normalized_movement_vector.x = 0.0f;
        normalized_movement_vector.y = 0.0f;
    }
}

void cursor_pos_callback(GLFWwindow *window, double x_pos, double y_pos) {

    look_h_angle += (window_width / 2 - x_pos) * 0.001f;
    look_v_angle -= (window_height / 2 - y_pos) * 0.001f;

    // Clamping, adding a small amount to prevent bugs when sine is equal zero.
    look_v_angle = clamp(look_v_angle, M_PI_2f32, M_PIf32 - 0.00001f);

    // Parametrization of a sphere by angles and radius (radius = 1)
    look_direction = glm::vec3(
            sin(look_v_angle) * cos(look_h_angle),
            sin(look_v_angle) * sin(look_h_angle),
            cos(look_v_angle)
    );

    glfwSetCursorPos(window, window_width/2, window_height/2);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

    window_width = width;
    window_height = height;

    // Adjust for the change in window ratio
    glm::mat4 projection_trans = glm::perspective(
            glm::radians(45.0f), (float) width / height, 1.0f, 50.0f);
    GLint uniform_projection = glGetUniformLocation(shader_program, "projection");
    glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection_trans));

    glViewport(0, 0, width, height);
}
#pragma clang diagnostic pop

static float clamp(float value, float min, float max) {
    if (value < min) {
        value = min;
    } else if (value > max) {
        value = max;
    }
    return value;
}
