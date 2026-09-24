#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "camera/FlyingCamera.h"
#include "graphics/Shader.h"
#include "pipeline/VertexArray.h"
#include "utils/Clock.h"

static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

static void mouse_callback(GLFWwindow *window, double xpos, double ypos);

static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

static void processInput(GLFWwindow *window, float delta_time);

// settings
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

// camera stuff
static std::unique_ptr<Camera> camera = std::make_unique<FlyingCamera>();
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool firstMouse = true;

// shapes
constexpr float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

// light
static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

static GLFWwindow *init_window() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    camera->setViewport(SCR_WIDTH, SCR_HEIGHT);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("Failed to initialize GLAD");
    }
    return window;
}

int main() {
    const auto window = init_window();

    // build and compile our shader program
    // ------------------------------------
    const Shader lighting_shader("shaders/light.vs", "shaders/light.fs");
    const Shader cube_shader("shaders/cube.vs", "shaders/cube.fs");

    glEnable(GL_DEPTH_TEST);

    {
        constexpr std::size_t stride = 5 * sizeof(float);
        std::vector<VertexAttribute> const position_layout{
            {.location = 0, .components = 3, .offset = 0, .type = attribute_type::float32}
        };

        VertexArray cube_vao;
        auto cube_buffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices), BufferUsage::static_draw);
        cube_vao.add_buffer(cube_buffer, position_layout, stride);

        VertexArray light_cube_vao;
        light_cube_vao.add_buffer(cube_buffer, position_layout, stride);

        while (!glfwWindowShouldClose(window)) {
            const auto delta_time = Clock::get_elapsed_time();
            processInput(window, delta_time);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Shader::use(lighting_shader);
            lighting_shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            lighting_shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

            // view/projection transformations
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom),
                                                    static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT),
                                                    0.1f, 100.0f);
            glm::mat4 view = camera->view();
            lighting_shader.setMat4("projection", projection);
            lighting_shader.setMat4("view", view);

            // world transformation
            auto model = glm::mat4(1.0f);
            lighting_shader.setMat4("model", model);

            // render the cube
            cube_vao.draw();


            // also draw the lamp object
            Shader::use(cube_shader);
            cube_shader.setMat4("projection", projection);
            cube_shader.setMat4("view", view);
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
            cube_shader.setMat4("model", model);

            light_cube_vao.draw();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    } // GL objects are released here, while the context is still alive

    glfwTerminate();
    return 0;
}

static void processInput(GLFWwindow *window, float delta_time) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->process_keyboard(Direction::FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->process_keyboard(Direction::BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->process_keyboard(Direction::LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->process_keyboard(Direction::RIGHT, delta_time);
}

void framebuffer_size_callback(GLFWwindow *window, const int width, const int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    if (camera) camera->setViewport(width, height);
}

void mouse_callback(GLFWwindow *window, const double xpos, const double ypos) {
    const auto x = static_cast<float>(xpos);
    const auto y = static_cast<float>(ypos);

    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    const float xoffset = x - lastX;
    const float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top

    lastX = x;
    lastY = y;

    camera->process_mouse_movement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, const double yoffset) {
    camera->process_mouse_scroll(static_cast<float>(yoffset));
}
