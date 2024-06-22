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

glm::vec3 Entity::GetPosition()
{
    return _position;
}

glm::vec3 Entity::GetRotation()
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
