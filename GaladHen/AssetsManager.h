
#pragma once

#include <map>
#include <string>

#include "Model.h"
#include "TextureImage.h"

namespace GaladHen
{
    class AssetsManager
    {

    public:

        AssetsManager() = delete;
        AssetsManager(AssetsManager& Other) = delete;
        AssetsManager& operator=(AssetsManager& Other) = delete;
        AssetsManager(AssetsManager&& Other) = default;
        AssetsManager& operator=(AssetsManager&& Other) noexcept = default;

        // @brief
        // Free the resources associated to the saved assets
        static void FreeAssets();

        // @brief
        // Load a model from a file and make it owned by assets manager
        static Model* LoadAndStoreModel(const std::string& modelPath, const std::string& modelName);

        // @brief
        // Load a texture image from a file and make it owned by assets manager
        static TextureImage* LoadAndStoreTexImage(const std::string& texImgPath, const std::string& texImgName);

        // @brief
        // Get a reference to a model owned by assets manager by its name
        static Model* GetModelByName(const std::string& assetName);

        // @brief
        // Get a reference to a texture image owned by assets manager by its name
        static TextureImage* GetTexImageByName(const std::string& assetName);

    protected:

        static std::map<const std::string, Model> Models;
        static std::map<const std::string, TextureImage> TextureImages;

    };
}
