#pragma once

#include <glad/glad.h>

class ModelTexture
{
    public:
        ModelTexture(GLuint id, float shineDamper, float reflectivity);
        GLuint GetTextureID();

        float GetShineDamper() const { return _shineDamper; }
        float GetReflectivity() const { return _reflectivity; }
        
    private:
        GLuint _textureId;
        float _shineDamper;
        float _reflectivity;
};