#include <component/model_texture.hpp>

ModelTexture::ModelTexture(GLuint id, float shineDamper, float reflectivity): _textureId(id), _shineDamper(shineDamper), _reflectivity(reflectivity)
{
    
}

ModelTexture::ModelTexture(const ModelTexture& mt)
{
    _textureId = mt._textureId;
    _reflectivity = mt._reflectivity;
    _shineDamper = mt._shineDamper;
}

GLuint ModelTexture::GetTextureID()
{
    return _textureId;
}
