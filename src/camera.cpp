#include <component/camera.hpp>
#include <iostream>

Camera::Camera(const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp, float yaw, float pitch)
    : _cameraPos(cameraPos),
      _cameraFront(cameraFront),
      _cameraUp(cameraUp),
      _yaw(yaw),
      _pitch(pitch)
{
}

