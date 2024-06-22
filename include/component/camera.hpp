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
        
    private:
        glm::vec3 _cameraPos;
        glm::vec3 _cameraFront;
        glm::vec3 _cameraUp;

        float _yaw;
        float _pitch;

        friend Window;

};