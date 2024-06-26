#include <component/camera.hpp>
#include <iostream>

Camera::Camera(const glm::vec3& cameraWorldPosition, const glm::vec3& viewDirection, float viewAzimuth, float viewElevation)
    : _cameraWorldPosition(cameraWorldPosition),
      _viewDirection(viewDirection),
      _viewAzimuth(viewAzimuth),
      _viewElevation(viewElevation), 
      _rightDirection(glm::vec3(0, 0, 0)), 
      _upDirection(glm::vec3(0, 1, 0)),
      _worldUpVector(glm::vec3(0, 1, 0))
{
}

