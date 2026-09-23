#ifndef LEARNOPENGL_FLYINGCAMERA_H
#define LEARNOPENGL_FLYINGCAMERA_H

#include "Camera.h"

/**
 * Flying God Camera.
 */
class FlyingCamera final : public Camera {
public:
    float speed = 2.5f;

    void update(float delta_time) override;

    [[nodiscard]] glm::vec3 position() const override { return pos; }

protected:
    [[nodiscard]] glm::mat4 view() const override;

    glm::vec3 poll_camera_input() override;

private:
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 6.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(front, up));
};

#endif //LEARNOPENGL_FLYINGCAMERA_H
