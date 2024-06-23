#pragma once

#include <glad/glad.h>

class RawModel
{
    public:
        RawModel(GLuint vaoID, int vertexCount);

        RawModel(const RawModel& rm);

        int GetVaoID();
        int GetVertexCount();

    private:
        int _vaoID;
        int _vertexCount;
};