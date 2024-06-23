#include <component/renderer.hpp>
#include <glad/glad.h>
#include <component/textured_model.hpp>
#include <component/raw_model.hpp>
#include <component/model_texture.hpp>
#include <component/entity.hpp>
#include <util/math.hpp>
#include <component/static_shader.hpp>
#include <cassert>

Renderer::Renderer(StaticShader* shader, int width, int height)
    : _shader(shader)
{
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

    _projectionDetails = std::make_unique<ProjectionDetails>(70.0f, width, height, 0.1f, 1000.0f);

    shader->Start();
    shader->LoadProjectionMatrix(Math::CreateProjectionMatrix(_projectionDetails.get()));
    shader->Stop();
}

void Renderer::Prepare()
{
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render(const std::unordered_map<std::string, std::vector<Entity*>>& _entities)
{
    for(const auto& pair: _entities)
    {
        this->PrepareTexturedModel(pair.second[0]->GetModel());
        
        for(const auto& entity: pair.second)
        {
            this->PrepareEntity(entity);
            glDrawElements(GL_TRIANGLES, entity->GetModel()->GetRawModel()->GetVertexCount(), GL_UNSIGNED_INT, 0);
        }

        this->UnBindTexturedModel();
    }
}

void Renderer::PrepareTexturedModel(TexturedModel* model)
{
    RawModel* rawModel = model->GetRawModel();
    ModelTexture* texture = model->GetTexture();

    glBindVertexArray(rawModel->GetVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    _shader->LoadShineVariables(texture->GetShineDamper(), texture->GetReflectivity());

    glActiveTexture(GL_TEXTURE0); /* corresponds to sampler2D uniform in vertex shader */
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
}

void Renderer::UnBindTexturedModel()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

void Renderer::PrepareEntity(Entity* entity)
{
    _shader->LoadModelMatrix(Math::CreateTransformationMatrix(entity->GetPosition(), entity->GetRotation(), entity->GetScale()));
}

// void Renderer::Render(Entity* entity)
// {
//     /* assert will be called when given condition is true */
//     assert(entity != nullptr);

//     TexturedModel* model = entity->GetModel();

//     RawModel* rawModel = model->GetRawModel();
//     ModelTexture* texture = model->GetTexture();

//     glBindVertexArray(rawModel->GetVaoID());
//     glEnableVertexAttribArray(0);
//     glEnableVertexAttribArray(1);
//     glEnableVertexAttribArray(2);

//     auto modelMatrix = Math::CreateTransformationMatrix(entity->GetPosition(), entity->GetRotation(), entity->GetScale());
//     shader->LoadModelMatrix(modelMatrix);

//     shader->LoadShineVariables(texture->GetShineDamper(), texture->GetReflectivity());

//     glActiveTexture(GL_TEXTURE0); /* corresponds to sampler2D uniform in vertex shader */
//     glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
//     glDrawElements(GL_TRIANGLES, rawModel->GetVertexCount(), GL_UNSIGNED_INT, 0);
//     glDisableVertexAttribArray(0);
//     glDisableVertexAttribArray(1);
//     glDisableVertexAttribArray(2);
//     glBindVertexArray(0);
// }


