#include <component/master_renderer.hpp>
#include <component/static_shader.hpp>
#include <component/renderer.hpp>
#include <component/textured_model.hpp>
#include <component/light.hpp>
#include <component/camera.hpp>
#include <component/entity.hpp>
#include <util/math.hpp>

MasterRenderer::MasterRenderer(int width, int height)
{
    _shader = std::make_unique<StaticShader>();
    _renderer = std::make_unique<Renderer>(_shader.get(), width, height);
}

void MasterRenderer::Render(Light* light, Camera* camera)
{
    _renderer->Prepare();
    _shader->Start();
	_shader->LoadCameraPosition(camera->GetWorldPosition());
    _shader->LoadLight(light);
    _shader->LoadViewMatrix(Math::CreateViewMatrix(camera));

    _renderer->Render(_entities);

    _shader->Stop();

    _entities.clear();  
}

void MasterRenderer::ProcessEntity(Entity* entity)
{
    TexturedModel* model = entity->GetModel();
    auto it = _entities.find(model->GetString());
    if(it != _entities.end())
    {
        it->second.emplace_back(entity);
    }
    else
    {
        _entities[model->GetString()].emplace_back(entity);
    }
}

void MasterRenderer::CleanUp()
{
    _shader->CleanUp();
}


