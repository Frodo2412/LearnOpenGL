#include "FlyingCamera.h"

#include <GLFW/glfw3.h>

#include "glm/ext/matrix_transform.hpp"

void FlyingCamera::update(const float delta_time) {
    pos += speed * delta_time * poll_camera_input();
}

glm::vec3 FlyingCamera::poll_camera_input() {
    GLFWwindow *window = glfwGetCurrentContext();

    auto displacement = glm::vec3(0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) displacement += front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) displacement -= front;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) displacement += right;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) displacement -= right;
    return displacement;
}

glm::mat4 FlyingCamera::view() const {
    return glm::lookAt(pos, pos + front, up);
}
