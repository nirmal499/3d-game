#pragma once

#include <memory>
#include <string>

class RawModel;
class ModelTexture;

class TexturedModel
{
    public:
        TexturedModel(std::unique_ptr<RawModel> model, std::unique_ptr<ModelTexture> texture, std::string name);

        TexturedModel(const TexturedModel& tm);
        
        RawModel* GetRawModel();
        ModelTexture* GetTexture();
        const char* GetString();

    private:
        std::unique_ptr<RawModel> _model;
        std::unique_ptr<ModelTexture> _texture;
        std::string _name;

};