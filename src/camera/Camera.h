#ifndef LEARNOPENGL_CAMERA_H
#define LEARNOPENGL_CAMERA_H
#include "glm/glm.hpp"

#include "graphics/Shader.h"

class Camera {
public:
    virtual ~Camera() = default;

    virtual void update(float delta_time) = 0;

    // Uploads the "view" and "projection" uniforms. The shader must be in use.
    void apply(const Shader &shader) const;

    // Call once at startup and on every framebuffer resize.
    void setViewport(int width, int height);

    [[nodiscard]] virtual glm::vec3 position() const = 0;
protected:

    [[nodiscard]] virtual glm::mat4 view() const = 0;

    // Perspective by default; orthographic cameras override.
    [[nodiscard]] virtual glm::mat4 projection() const;

    float aspect = 4.0f / 3.0f;

    // Reads whatever input this camera cares about and returns the resulting displacement direction.
    virtual glm::vec3 poll_camera_input() = 0;
};

#endif //LEARNOPENGL_CAMERA_H
