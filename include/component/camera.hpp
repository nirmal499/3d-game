#pragma once

#include <window/window.hpp>
#include <util/common.hpp>

class Camera
{
    public:
        Camera(const glm::vec3& cameraWorldPosition, const glm::vec3& viewDirection, const glm::vec3& worldUpVector, float yaw, float pitch);

        glm::vec3 GetWorldPosition() const { return _cameraWorldPosition; }
        glm::vec3 GetViewDirection() const { return _viewDirection; }
        glm::vec3 GetWorldUpDirection() const { return _worldUpVector; }

    private:
        glm::vec3 _cameraWorldPosition;
        glm::vec3 _viewDirection;

        /* world up is positive Y */
        glm::vec3 _worldUpVector;

        float _yaw;
        float _pitch;

        friend Window;
};