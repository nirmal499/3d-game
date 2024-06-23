#include <component/textured_model.hpp>
#include <component/raw_model.hpp>
#include <component/model_texture.hpp>


TexturedModel::TexturedModel(std::unique_ptr<RawModel> model, std::unique_ptr<ModelTexture> texture, std::string name)
    : _model(std::move(model)),
      _texture(std::move(texture)), _name(std::move(name))
{
    
}

TexturedModel::TexturedModel(const TexturedModel& tm)
{
    _model = std::make_unique<RawModel>(*tm._model);
    _texture = std::make_unique<ModelTexture>(*tm._texture);
    _name = tm._name;
}

RawModel* TexturedModel::GetRawModel()
{
    return _model.get();    
}

ModelTexture* TexturedModel::GetTexture()
{
    return _texture.get();
}

const char* TexturedModel::GetString()
{
    return _name.c_str();
}

