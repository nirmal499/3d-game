#pragma once

#include <glad/glad.h>

class RawModel
{
    public:
        RawModel(GLuint vaoID, int vertexCount);

        int GetVaoID();
        int GetVertexCount();

    private:
        int _vaoID;
        int _vertexCount;
};