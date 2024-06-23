#include <component/textured_model.hpp>
#include <component/raw_model.hpp>
#include <component/model_texture.hpp>


TexturedModel::TexturedModel(std::unique_ptr<RawModel> model, std::unique_ptr<ModelTexture> texture)
    : _model(std::move(model)),
      _texture(std::move(texture))
{
    
}

RawModel* TexturedModel::GetRawModel()
{
    return _model.get();    
}

ModelTexture* TexturedModel::GetTexture()
{
    return _texture.get();
}

