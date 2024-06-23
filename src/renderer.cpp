#include <component/renderer.hpp>
#include <glad/glad.h>
#include <component/textured_model.hpp>
#include <component/raw_model.hpp>
#include <component/model_texture.hpp>
#include <component/entity.hpp>
#include <util/math.hpp>
#include <component/static_shader.hpp>
#include <cassert>

void Renderer::Prepare()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Renderer::Renderer()
{
    
}

void Renderer::Render(Entity* entity, StaticShader* shader)
{
    /* assert will be called when given condition is true */
    assert(entity != nullptr);

    TexturedModel* model = entity->GetModel();

    RawModel* rawModel = model->GetRawModel();
    ModelTexture* texture = model->GetTexture();

    glBindVertexArray(rawModel->GetVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    auto modelMatrix = Math::CreateTransformationMatrix(entity->GetPosition(), entity->GetRotation(), entity->GetScale());
    shader->LoadModelMatrix(modelMatrix);

    shader->LoadShineVariables(texture->GetShineDamper(), texture->GetReflectivity());

    glActiveTexture(GL_TEXTURE0); /* corresponds to sampler2D uniform in vertex shader */
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    glDrawElements(GL_TRIANGLES, rawModel->GetVertexCount(), GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}


