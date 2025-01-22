
#pragma once

#include <string>
#include <memory>

namespace GaladHen
{
    class Model;
    class Texture;
    class ShaderPipeline;
    enum class TextureFormat;

    namespace AssetsManager
    {
        // @brief
        // Load a model from a file and make it owned by assets manager
        std::shared_ptr<Model> LoadAndStoreModel(const std::string& modelPath, const std::string& modelName);

        // @brief
        // Load a texture from a file and make it owned by assets manager
        std::shared_ptr<Texture> LoadAndStoreTexture(const std::string& texturePath, const std::string& textureName, TextureFormat textureFormat);

        // @brief
        // Load a shader pipeline from a shader files and make it owned by assets manager
        std::shared_ptr<ShaderPipeline> LoadAndStoreShaderPipeline(
            const std::string& vShaderPath,
            const std::string& tContShaderPath,
            const std::string& tEvalShaderPath,
            const std::string& gShaderPath,
            const std::string& fShaderPath,
            const std::string& cShaderPath,
            const std::string& pipelineName);

        // @brief
        // Get a reference to a model owned by assets manager by its name
        std::shared_ptr<Model> GetModelByName(const std::string& modelName);

        // @brief
        // Get a reference to a texture owned by assets manager by its name
        std::shared_ptr<Texture> GetTextureByName(const std::string& textureName);

        // @brief
        // Get a reference to a shader pipeline owned by assets manager by its name
        std::shared_ptr<ShaderPipeline> GetShaderPipelineByName(const std::string& pipelineName);

        // @brief
        // Store a copy of the source model, making it owned by the assets manager
        std::shared_ptr<Model> StoreModel(const Model& model, const std::string& modelName);

        // @brief
        // Free the resources associated to the saved assets
        void FreeAssets();
    };
}