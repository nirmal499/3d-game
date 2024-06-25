#pragma once

#include <window/window.hpp>
#include <component/entity.hpp>

class Player : public Entity
{
    public:
        Player(std::unique_ptr<TexturedModel> model, const glm::vec3& position, const glm::vec3& rotation, float scale);

    private:
        static float RUN_SPEED;
        static float TURN_SPEED;

        float _currentSpeed = 0;
        float _currentTurnSpeed = 0;

        friend Window;

};