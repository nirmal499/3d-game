#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

class Renderer;
class StaticShader;
class Entity;
class Light;
class Camera;

class MasterRenderer
{
    public:
        MasterRenderer(int width, int height);
        void Render(Light* light, Camera* camera);
        void CleanUp();
        void ProcessEntity(Entity* entity);

    private:
        std::unique_ptr<StaticShader> _shader;
        std::unique_ptr<Renderer> _renderer;
        std::unordered_map<std::string, std::vector<Entity*>> _entities;
};