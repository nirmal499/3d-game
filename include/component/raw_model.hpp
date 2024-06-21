#pragma once

#include <glad/glad.h>

class RawModel
{
    public:
        RawModel(GLuint vaoID, int vertexCount);

        int getVaoID();
        int getVertexCount();

    private:
        int _vaoID;
        int _vertexCount;
};