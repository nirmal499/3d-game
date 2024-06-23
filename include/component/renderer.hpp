#pragma once

#include <util/common.hpp>

class Entity;
class StaticShader;
class TexturedModel;

class Renderer
{
    public:
        Renderer(StaticShader* shader, int width, int height);
        /* It will be called every frame */
        void Prepare();
        // void Render(Entity* entity);
        void Render(const std::unordered_map<std::string, std::vector<Entity*>>& _entities);
        void PrepareTexturedModel(TexturedModel* model);
        void UnBindTexturedModel();
        void PrepareEntity(Entity* entity);

    private:
        StaticShader* _shader;
	    std::unique_ptr<ProjectionDetails> _projectionDetails;

};