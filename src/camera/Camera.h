#ifndef LEARNOPENGL_CAMERA_H
#define LEARNOPENGL_CAMERA_H
#include "Directions.h"
#include "glm/glm.hpp"

#include "graphics/Shader.h"

class Camera {
public:
    float Zoom = 45.0f;

    virtual ~Camera() = default;

    // Uploads the "view" and "projection" uniforms. The shader must be in use.
    void apply(const Shader &shader) const;

    // Call once at startup and on every framebuffer resize.
    void setViewport(int width, int height);

    [[nodiscard]] virtual glm::vec3 position() const = 0;

    virtual void process_keyboard(Direction direction, float delta_time) {}
    virtual void process_mouse_movement(float x_offset, float y_offset) {}
    virtual void process_mouse_scroll(float y_offset) {}

    [[nodiscard]] virtual glm::mat4 view() const = 0;

protected:

    // Perspective by default; orthographic cameras override.
    [[nodiscard]] virtual glm::mat4 projection() const;

    float aspect = 4.0f / 3.0f;
    float fov = 45.0f;
};

#endif //LEARNOPENGL_CAMERA_H
