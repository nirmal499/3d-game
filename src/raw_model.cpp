#include <component/raw_model.hpp>

RawModel::RawModel(GLuint vaoID, int vertexCount)
    : _vaoID(vaoID),
      _vertexCount(vertexCount)
{
    
}

int RawModel::GetVaoID()
{
    return _vaoID;
}

int RawModel::GetVertexCount()
{
    return _vertexCount;
}