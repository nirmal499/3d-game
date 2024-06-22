#pragma once

#include <memory>
#include <util/common.hpp>

class TexturedModel;

class Entity
{
    public:
        Entity(std::unique_ptr<TexturedModel> model, const glm::vec3& position, const glm::vec3& rotation, float scale);
        
        glm::vec3 GetPosition();
        glm::vec3 GetRotation();
        float GetScale();

        void IncreasePosition(const glm::vec3& change);
        void IncreaseRotation(const glm::vec3& change);

        TexturedModel* GetModel();
    
    private:
        std::unique_ptr<TexturedModel> _model;
        glm::vec3 _position;
        glm::vec3 _rotation;
        float _scale;

};