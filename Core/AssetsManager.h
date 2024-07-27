
#pragma once

#include <map>
#include <string>

namespace GaladHen
{
    class Model;
    class Texture;
    class Shader;
    class ShaderPipeline;
    class ComputeShader;
    enum ShaderStage;
    enum class TextureFormat;

    class AssetsManager
    {

    public:

        AssetsManager() = delete;
        AssetsManager(const AssetsManager& Other) = delete;
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
        // Load a texture from a file and make it owned by assets manager
        static Texture* LoadAndStoreTexture(const std::string& texImgPath, const std::string& texImgName, TextureFormat textureFormat);

        // @brief
       // Load a shader from a file and make it owned by assets manager
        static Shader* LoadAndStoreShader(const std::string& shaderPath, const std::string& shaderName, ShaderStage shaderStage);

        // @brief
        // Get a reference to a model owned by assets manager by its name
        static Model* GetModelByName(const std::string& assetName);

        // @brief
        // Get a reference to a texture owned by assets manager by its name
        static Texture* GetTextureByName(const std::string& assetName);

        // DEFAULTS --------------------------------------------------------------------------------------------------

        // @brief
        // Getter for default pbr shader pipeline
        static ShaderPipeline GetPipelinePBR();

    protected:

        // General assets
        static std::map<const std::string, Model> Models;
        static std::map<const std::string, Texture> Textures;
        static std::map<const std::string, Shader> Shaders;

        // Default assets

        // Shaders

        // PBR
        static Shader PBR_VertexShader;
        static Shader PBR_FragmentShader;

        // Models

        // Textures

    };
}
