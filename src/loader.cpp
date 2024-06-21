#include <component/loader.hpp>
#include <component/raw_model.hpp>

std::unique_ptr<RawModel> Loader::LoadToVao(const std::vector<float>& positions, const std::vector<unsigned int>& indices)
{
    GLuint VaoID;
    GLuint VboID;
    GLuint EboID;

    glGenVertexArrays(1, &VaoID);
    _vaos.push_back(VaoID);

    glGenBuffers(1, &VboID);
    _vbos.push_back(VboID);

    glGenBuffers(1, &EboID);
    _vbos.push_back(EboID);

    glBindVertexArray(VaoID);

        glBindBuffer(GL_ARRAY_BUFFER, VboID);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), indices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
            glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->UnBindVao(); /* glBindVertexArray(0) */

    return std::make_unique<RawModel>(VaoID, indices.size());

    /* Logger::log(1, "%s: VAO and VBO initialized\n", __FUNCTION__); */
}

void Loader::UnBindVao()
{
    glBindVertexArray(0);
}

void Loader::CleanUp()
{
    for(GLuint id: _vaos)
    {
        glDeleteVertexArrays(1, &id);
    }

    for(GLuint id: _vbos)
    {
        glDeleteBuffers(1, &id);
    }
}

