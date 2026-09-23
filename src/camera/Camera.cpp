#include "Camera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/trigonometric.hpp"

void Camera::apply(const Shader &shader) const {
    shader.setMat4("view", view());
    shader.setMat4("projection", projection());
}

void Camera::setViewport(const int width, const int height) {
    if (height > 0) aspect = static_cast<float>(width) / static_cast<float>(height);
}

glm::mat4 Camera::projection() const {
    return glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}
