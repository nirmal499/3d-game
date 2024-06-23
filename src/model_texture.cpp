#include <component/model_texture.hpp>

ModelTexture::ModelTexture(GLuint id, float shineDamper, float reflectivity): _textureId(id), _shineDamper(shineDamper), _reflectivity(reflectivity)
{
    
}

GLuint ModelTexture::GetTextureID()
{
    return _textureId;
}
