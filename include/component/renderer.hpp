#pragma once

#include <util/common.hpp>

class Entity;
class StaticShader;

class Renderer
{
    public:
        Renderer();
        /* It will be called every frame */
        void Prepare();
        void Render(Entity* entity, StaticShader* shader);
};