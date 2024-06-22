#pragma once

#include <memory>

class RawModel;
class Loader;

class OBJLoader
{
    public:
        std::unique_ptr<RawModel> LoadObjModel(const char* filePath, Loader* loader);
};