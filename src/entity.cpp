#include <component/entity.hpp>
#include <component/textured_model.hpp>
#include <component/raw_model.hpp>
#include <component/model_texture.hpp>

Entity::Entity(std::unique_ptr<TexturedModel> model, const glm::vec3& position, const glm::vec3& rotation, float scale)
    : _model(std::move(model)),
      _position(position),
      _rotation(rotation),
      _scale(scale)
{
    
}

const glm::vec3& Entity::GetPosition()
{
    return _position;
}

void Entity::SetPosition(const std::array<float, 3>& position)
{
    _position.x = position[0];
    _position.y = position[1];
    _position.z = position[2];
}

void Entity::SetRotation(const std::array<float, 3>& rotation)
{
    _rotation.x = rotation[0];
    _rotation.y = rotation[1];
    _rotation.z = rotation[2];
}

const glm::vec3& Entity::GetRotation()
{
    return _rotation;
}

float Entity::GetScale()
{
    return _scale;
}

void Entity::IncreasePosition(const glm::vec3& change)
{
    _position += change;
}

void Entity::IncreaseRotation(const glm::vec3& change)
{
    _rotation += change;
}

TexturedModel* Entity::GetModel()
{
    return _model.get();
}
