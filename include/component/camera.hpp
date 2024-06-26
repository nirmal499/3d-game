#pragma once

#include <window/window.hpp>
#include <util/common.hpp>

class Camera
{
    public:
        Camera(const glm::vec3& cameraWorldPosition, const glm::vec3& viewDirection, float viewAzimuth, float viewElevation);

        glm::vec3 GetWorldPosition() const { return _cameraWorldPosition; }
        glm::vec3 GetViewDirection() const { return _viewDirection; }
        glm::vec3 GetRightDirection() const { return _rightDirection; }
        glm::vec3 GetUpDirection() const { return _upDirection; }

    private:
        glm::vec3 _cameraWorldPosition;
        glm::vec3 _viewDirection;
        glm::vec3 _rightDirection;
        glm::vec3 _upDirection;

        /* world up is positive Y */
        glm::vec3 _worldUpVector;

        float _viewAzimuth;
        float _viewElevation;

        friend Window;
};