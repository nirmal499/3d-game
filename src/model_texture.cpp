#include <component/model_texture.hpp>

ModelTexture::ModelTexture(GLuint id): _textureId(id)
{
    
}

GLuint ModelTexture::getTextureID()
{
    return _textureId;
}
