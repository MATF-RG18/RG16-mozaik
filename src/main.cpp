#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define GLEW_STATIC
#pragma clang diagnostic pop

#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>

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

    // 20 lines * 2 vertices each * 3 attributes each
    float vertices[20 * 2 * 3];

    for (int i = 0; i < 10; i++) {
        // stride is 3 * 4 <=> 4 vertices have 12 attributes in total
        int offset = i * 3 * 4;

        // Start point
        vertices[offset] = -1.0f + i * 0.2f;        // X
        vertices[offset + 1] = -1.0f;               // Y
        vertices[offset + 2] = 0.0f;                // Z

        // End point
        vertices[offset + 3] = -1.0f + i * 0.2f;    // X
        vertices[offset + 4] = 1.0f;                // Y
        vertices[offset + 5] = 0.0f;                // Z

        // Start point
        vertices[offset + 6] = -1.0f;                // X
        vertices[offset + 7] = -1.0f + i * 0.2f;     // Y
        vertices[offset + 8] = 0.0f;                 // Z

        // End point
        vertices[offset + 9] = 1.0f;                  // X
        vertices[offset + 10] = -1.0f + i * 0.2f;     // Y
        vertices[offset + 11] = 0.0f;                 // Z
    }

    // Static draw because data is written once and used many times.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Initialize shaders
    GLuint shader_program = init_shaders();

    // Make a connection between vertex data and attributes
    GLuint position_attribute = static_cast<GLuint>(glGetAttribLocation(shader_program, "position"));
    glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(position_attribute);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glDrawArrays(GL_LINES, 0, 40);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    // TODO: return glError for debugging purposes
    return 0;
}

GLuint init_shaders() {
    // TODO: move to a file in the future
    const char* vertex_shader_source = R"glsl(
        #version 150 core

        in vec3 position;

        void main() {
            /* Fourth coordinate is related to clipping and should default to 1.0 */
            gl_Position = vec4(position, 1.0);
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

        void main() {
            /* Just white for now */
            out_color = vec4(1.0, 1.0, 1.0, 1.0);
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