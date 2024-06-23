#pragma once

#include <util/common.hpp>

class Light
{
    public:
        Light(const glm::vec3& position,const glm::vec3& colour);

    public:
        glm::vec3 GetPosition() const { return _position; }
        glm::vec3 GetColour() const { return _colour; }

    private:
        glm::vec3 _position;
        glm::vec3 _colour; /* light intensity */
};