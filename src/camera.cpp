#include <component/camera.hpp>
#include <iostream>

Camera::Camera(const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp)
    : _cameraPos(cameraPos),
      _cameraFront(cameraFront),
      _cameraUp(cameraUp)
{
}

