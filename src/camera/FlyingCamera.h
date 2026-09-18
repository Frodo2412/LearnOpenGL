#ifndef LEARNOPENGL_FLYINGCAMERA_H
#define LEARNOPENGL_FLYINGCAMERA_H
#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include "glm/ext/matrix_transform.hpp"


class FlyingCamera {
public:
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f),
            cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f),
            cameraDirection = glm::normalize(cameraPos - cameraTarget),
            up = glm::vec3(0.0f, 1.0f, 0.0f),
            cameraRight = glm::normalize(glm::cross(up, cameraDirection)),
            cameraUp = glm::cross(cameraDirection, cameraRight),
            cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));


    FlyingCamera();
};


#endif //LEARNOPENGL_FLYINGCAMERA_H
