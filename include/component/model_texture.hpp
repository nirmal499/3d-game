#pragma once

#include <glad/glad.h>

class ModelTexture
{
    public:
        ModelTexture(GLuint id);
        GLuint getTextureID();
        
    private:
        GLuint _textureId;
};