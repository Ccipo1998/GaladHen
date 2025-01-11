
#pragma once

#include <string>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace GaladHen
{
    class Model;
    class Texture;
    enum class TextureFormat;

    namespace AssetLoader
    {
        Model* LoadModelFromFile(const char* filePath);

        Texture* LoadTextureFromFile(const char* filePath, TextureFormat textureFormat);
    };
}