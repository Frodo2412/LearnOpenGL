#ifndef LEARNOPENGL_FLYINGCAMERA_H
#define LEARNOPENGL_FLYINGCAMERA_H

#include "Camera.h"

/**
 * Flying God Camera.
 */
class FlyingCamera final : public Camera {
public:
    float speed = 2.5f;
    float mouse_sensitivity = 0.1f;

    [[nodiscard]] glm::vec3 position() const override { return pos; }

    void process_keyboard(Direction direction, float delta_time) override;

    void process_mouse_movement(float x_offset, float y_offset) override;

    void process_mouse_scroll(float y_offset) override;

protected:
    [[nodiscard]] glm::mat4 view() const override;

private:
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 6.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(front, up));

    float yaw = -90.0f;
    float pitch = 0.0f;

    // Recomputes front/right/up from yaw and pitch.
    void update_vectors();
};

#endif //LEARNOPENGL_FLYINGCAMERA_H
