#pragma once

#include <util/common.hpp>

class Camera;

struct Math
{
    static glm::mat4 CreateTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, float scale);
    static glm::mat4 CreateProjectionMatrix(ProjectionDetails* projectionDetails);
    static glm::mat4 CreateViewMatrix(Camera* camera);
};