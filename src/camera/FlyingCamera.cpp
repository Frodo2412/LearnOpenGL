//
// Created by Bruno Lemus on 18/9/26.
//

#include "FlyingCamera.h"

FlyingCamera::FlyingCamera() = default;

void FlyingCamera::update_position(const float delta_time, const glm::vec3 displacement) {
    cameraPos += speed * delta_time * displacement;
}
