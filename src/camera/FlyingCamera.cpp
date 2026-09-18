//
// Created by Bruno Lemus on 18/9/26.
//

#include "FlyingCamera.h"

#include "utils/Clock.h"

FlyingCamera::FlyingCamera() = default;

void FlyingCamera::update_position(const glm::vec3 displacement) {
    const float delta_time = Clock::get_elapsed_time();
    cameraPos += speed * delta_time * displacement;
}

void FlyingCamera::update_camera(const glm::vec3 displacement) {
    update_position(displacement);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
