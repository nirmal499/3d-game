#include <component/renderer.hpp>
#include <glad/glad.h>
#include <component/textured_model.hpp>
#include <component/raw_model.hpp>
#include <component/model_texture.hpp>
#include <cassert>

void Renderer::Prepare()
{
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Render(TexturedModel* model)
{
    /* assert will be called when given condition is true */
    assert(model != nullptr);

    RawModel* rawModel = model->getRawModel();
    ModelTexture* texture = model->getTexture();

    glBindVertexArray(rawModel->getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glActiveTexture(GL_TEXTURE0); /* corresponds to sampler2D uniform in vertex shader */
    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
    glDrawElements(GL_TRIANGLES, rawModel->getVertexCount(), GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
}


