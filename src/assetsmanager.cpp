
#include <ezengine/assetsmanager.h>
#include <utility>
#include <iterator>
#include <utils/log.h>
#include <ezengine/model.h>
#include <ezengine/material.h>
#include <ezengine/textureimage.h>
#include <ezengine/texture.h>

AssetsManagerInternal::AssetsManagerInternal() {}

void AssetsManagerInternal::SaveModelWithName(Model* modelToSave, const std::string& assetName)
{
    const auto& res = this->Models.insert(std::pair<const std::string, Model*>(assetName, modelToSave));

    if (!res.second)
    {
        Log::Error("AssetsManager", "Tried to save a model with an already used name");
    }
}

Model* AssetsManagerInternal::GetModelByName(const std::string& assetName)
{
    return this->Models[assetName];
}

void AssetsManagerInternal::SaveMaterialWithName(Material* materialToSave, const std::string& assetName)
{
    const auto& res = this->Materials.insert(std::pair<const std::string, Material*>(assetName, materialToSave));

    if (!res.second)
    {
        Log::Error("AssetsManager", "Tried to save a material with an already used name");
    }
}

Material* AssetsManagerInternal::GetMaterialByName(const std::string& assetName)
{
    return this->Materials[assetName];
}

void AssetsManagerInternal::SaveTexImageWithName(TextureImage* texImgToSave, const std::string& assetName)
{
    const auto& res = this->TextureImages.insert(std::pair<const std::string, TextureImage*>(assetName, texImgToSave));

    if (!res.second)
    {
        Log::Error("AssetsManager", "Tried to save a texture image with an already used name");
    }
}

TextureImage* AssetsManagerInternal::GetTexImageByName(const std::string& assetName)
{
    return this->TextureImages[assetName];
}

void AssetsManagerInternal::SaveTextureWithName(Texture* textureToSave, const std::string& assetName)
{
    const auto& res = this->Textures.insert(std::pair<const std::string, Texture*>(assetName, textureToSave));

    if (!res.second)
    {
        Log::Error("AssetsManager", "Tried to save a texture with an already used name");
    }
}

Texture* AssetsManagerInternal::GetTextureByName(const std::string& assetName)
{
    return this->Textures[assetName];
}

AssetsManagerInternal::~AssetsManagerInternal()
{
    for (auto const& kv : this->Models)
    {
        delete kv.second;
    }

    for (auto const& kv : this->Materials)
    {
        delete kv.second;
    }

    for (auto const& kv : this->TextureImages)
    {
        delete kv.second;
    }

    for (auto const& kv : this->Textures)
    {
        delete kv.second;
    }
}
