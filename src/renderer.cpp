#include <component/renderer.hpp>
#include <glad/glad.h>
#include <component/raw_model.hpp>
#include <cassert>

void Renderer::Prepare()
{
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Render(RawModel* model)
{
    /* assert will be called when given condition is true */
    assert(model != nullptr);

    glBindVertexArray(model->getVaoID());
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, model->getVertexCount(), GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}


