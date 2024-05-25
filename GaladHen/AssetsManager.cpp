
#include "AssetsManager.h"
#include <utility>
#include <iterator>
#include <Utils/log.h>
#include "model.h"
#include "textureimage.h"

// assimp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace GaladHen
{

    // init
    std::map<const std::string, Model> AssetsManager::Models = std::map<const std::string, Model>{};
    std::map<const std::string, TextureImage> AssetsManager::TextureImages = std::map<const std::string, TextureImage>{};

    void AssetsManager::FreeAssets()
    {
        AssetsManager::Models.clear();
        AssetsManager::TextureImages.clear();
    }
    
    Model* AssetsManager::LoadAndStoreModel(const std::string& modelPath, const std::string& modelName)
    {
        const auto& res = AssetsManager::Models.emplace(std::pair<const std::string, Model>(modelName, Model{ modelPath }));

        if (!res.second)
        {
            Log::Error("AssetsManager", "Tried to save a model with an already used name");
            return nullptr;
        }

        return &(*res.first).second;
    }

    Model* AssetsManager::GetModelByName(const std::string& assetName)
    {
        return &Models[assetName];
    }

    TextureImage* AssetsManager::LoadAndStoreTexImage(const std::string& texImgPath, const std::string& texImgName)
    {
        const auto& res = AssetsManager::TextureImages.emplace(std::pair<const std::string, TextureImage>(texImgName, TextureImage{ texImgPath }));

        if (!res.second)
        {
            Log::Error("AssetsManager", "Tried to save a texture image with an already used name");
            return nullptr;
        }

        return &(*res.first).second;
    }

    TextureImage* AssetsManager::GetTexImageByName(const std::string& assetName)
    {
        return &TextureImages[assetName];
    }

}
