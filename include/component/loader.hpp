#pragma once

#include <memory>
#include <vector>
#include <glad/glad.h>

class RawModel;

class Loader
{
    public:
        std::unique_ptr<RawModel> LoadToVao(const std::vector<float>& positions, const std::vector<float>& textureCoords, const std::vector<float>& normals, const std::vector<unsigned int>& indices);
        GLuint LoadTexture(const char* fileName);
        void UnBindVao();
        void CleanUp();

    private:
        std::vector<GLuint> _vaos;
        std::vector<GLuint> _vbos;
        std::vector<GLuint> _textures;
};