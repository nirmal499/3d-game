#pragma once

#include <window/window.hpp>
#include <util/common.hpp>

class Camera
{
    public:
        Camera(const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp, float yaw, float pitch);

        glm::vec3 GetPositionVector() const { return _cameraPos; }
        glm::vec3 GetFrontVector() const { return _cameraFront; }
        glm::vec3 GetUpVector() const { return _cameraUp; }

        glm::vec3 GetWorldPosition() const { return _cameraWorldPosition; }
        glm::vec3 GetViewDirection() const { return _viewDirection; }
        glm::vec3 GetRightDirection() const { return _rightDirection; }
        glm::vec3 GetUpDirection() const { return _upDirection; }

    private:
        glm::vec3 _cameraPos;
        glm::vec3 _cameraFront;
        glm::vec3 _cameraUp;

        glm::vec3 _cameraWorldPosition = glm::vec3(0, 0, 3);
        glm::vec3 _viewDirection = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 _rightDirection = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 _upDirection = glm::vec3(0.0f, 0.0f, 0.0f);

        /* world up is positive Y */
        glm::vec3 _worldUpVector = glm::vec3(0.0f, 1.0f, 0.0f);

        float _viewAzimuth = 0.0f;
        float _viewElevation = 0.0f;

        float _yaw;
        float _pitch;

        friend Window;

};