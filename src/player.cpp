#include <component/player.hpp>
#include <component/model_texture.hpp>
#include <component/raw_model.hpp>
#include <component/textured_model.hpp>

float Player::RUN_SPEED = 5.0f;
float Player::TURN_SPEED = 5.0f;

Player::Player(std::unique_ptr<TexturedModel> model, const glm::vec3& position, const glm::vec3& rotation, float scale)
    : Entity(std::move(model), position, rotation, scale)
{
    
}
