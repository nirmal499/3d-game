#include <component/camera.hpp>
#include <iostream>

Camera::Camera(const glm::vec3& cameraWorldPosition, const glm::vec3& viewDirection, const glm::vec3& worldUpVector, float yaw, float pitch)
    : _cameraWorldPosition(cameraWorldPosition),
      _viewDirection(viewDirection),
      _worldUpVector(worldUpVector),
      _yaw(yaw), _pitch(pitch)
{
}

