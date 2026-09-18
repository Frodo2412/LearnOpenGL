#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/vertex.h"
#include "pipeline/vertex_array.h"
#include "pipeline/vertex_buffer.h"

static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

static void process_input(GLFWwindow *window);

// settings
constexpr unsigned int scr_width = 800;
constexpr unsigned int scr_height = 600;

int main(int argc, char *argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(scr_width, scr_height, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // ReSharper disable once CppCStyleCast
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) // NOLINT(clang-diagnostic-cast-function-type-strict)
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    const shader shader_program("./shaders/basic_vertex_shader.glsl", "./shaders/basic_fragment_shader.glsl");
    const texture texture("./assets/textures/container.jpg");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    const vertex vertices[] = {
        {0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
        {0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f},
        {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
        {-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f}
    };

    const unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };
    // GL objects owned by RAII types must be destroyed while the context is still alive,
    // so everything that touches GL lives in this scope, before glfwTerminate().
    {
        vertex_array quad;
        quad.add_buffer(vertex_buffer(vertices, sizeof(vertices)),
                        {
                            {.location = 0, .components = 3, .offset = 0 * sizeof(float)}, // position
                            {.location = 1, .components = 3, .offset = 3 * sizeof(float)}, // color
                            {.location = 2, .components = 2, .offset = 6 * sizeof(float)} // texture coord
                        },
                        sizeof(vertex));
        quad.set_indices(indices, 6);

        glBindTexture(GL_TEXTURE_2D, texture.get_id());

        while (!glfwWindowShouldClose(window)) {
            // input
            // -----
            process_input(window);

            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // render the quad
            shader_program.use();
            quad.draw();

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    } // quad destroyed here

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
