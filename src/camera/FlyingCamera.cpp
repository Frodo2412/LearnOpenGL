#include "FlyingCamera.h"

#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

void FlyingCamera::process_keyboard(Direction direction, float delta_time) {
    switch (direction) {
        case Direction::FORWARD:
            pos += front * delta_time;
            break;
        case Direction::BACKWARD:
            pos -= front * delta_time;
            break;
        case Direction::LEFT:
            pos -= right * delta_time;
            break;
        case Direction::RIGHT:
            pos += right * delta_time;
            break;
    }
}

void FlyingCamera::process_mouse_movement(const float x_offset, const float y_offset) {
    yaw += x_offset * mouse_sensitivity;
    pitch += y_offset * mouse_sensitivity;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    update_vectors();
}

void FlyingCamera::process_mouse_scroll(const float y_offset) {
    fov = glm::clamp(fov - y_offset, 1.0f, 45.0f);
}

void FlyingCamera::update_vectors() {
    front = glm::normalize(glm::vec3(
        glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
        glm::sin(glm::radians(pitch)),
        glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
    ));
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, front));
}

glm::mat4 FlyingCamera::view() const {
    return glm::lookAt(pos, pos + front, up);
}
