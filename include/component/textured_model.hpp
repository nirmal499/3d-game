#pragma once

#include <memory>

class RawModel;
class ModelTexture;

class TexturedModel
{
    public:
        TexturedModel(std::unique_ptr<RawModel> model, std::unique_ptr<ModelTexture> texture);
        RawModel* getRawModel();
        ModelTexture* getTexture();

    private:
        std::unique_ptr<RawModel> _model;
        std::unique_ptr<ModelTexture> _texture;

};