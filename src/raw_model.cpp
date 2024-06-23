#include <component/raw_model.hpp>

RawModel::RawModel(GLuint vaoID, int vertexCount)
    : _vaoID(vaoID),
      _vertexCount(vertexCount)
{
    
}

RawModel::RawModel(const RawModel& rm)
{
    _vaoID = rm._vaoID;
    _vertexCount = rm._vertexCount;
}

int RawModel::GetVaoID()
{
    return _vaoID;
}

int RawModel::GetVertexCount()
{
    return _vertexCount;
}