#include <component/raw_model.hpp>

RawModel::RawModel(GLuint vaoID, int vertexCount)
    : _vaoID(vaoID),
      _vertexCount(vertexCount)
{
    
}

int RawModel::getVaoID()
{
    return _vaoID;
}

int RawModel::getVertexCount()
{
    return _vertexCount;
}