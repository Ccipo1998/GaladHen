
#include "AssetsManager.h"
#include "Model.h"
#include "TextureImage.h"
#include "Shader.h"
#include "ShaderProgram.h"

#include <utility>
#include <iterator>
#include <Utils/Log.h>
#include <Utils/FileLoader.h>

namespace GaladHen
{

    // Inits
    std::map<const std::string, Model> AssetsManager::Models = std::map<const std::string, Model>{};
    std::map<const std::string, TextureImage> AssetsManager::TextureImages = std::map<const std::string, TextureImage>{};
    std::map<const std::string, Shader> AssetsManager::Shaders = std::map<const std::string, Shader>{};
    Shader AssetsManager::PBR_VertexShader = Shader("../Shaders/pbr/Pbr.vert", ShaderStage::Vertex); // default
    Shader AssetsManager::PBR_FragmentShader = Shader("../Shaders/pbr/Pbr.frag", ShaderStage::Fragment); // default

    void AssetsManager::FreeAssets()
    {
        AssetsManager::Models.clear();
        AssetsManager::TextureImages.clear();
    }
    
    Model* AssetsManager::LoadAndStoreModel(const std::string& modelPath, const std::string& modelName)
    {
        const auto& res = AssetsManager::Models.emplace(modelName, FileLoader::ReadModelFile(modelPath.data()));

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
        const auto& res = AssetsManager::TextureImages.emplace(texImgName, FileLoader::ReadImageFile(texImgPath.data()));

        if (!res.second)
        {
            Log::Error("AssetsManager", "Tried to save a texture image with an already used name");
            return nullptr;
        }

        return &(*res.first).second;
    }

    Shader* AssetsManager::LoadAndStoreShader(const std::string& shaderPath, const std::string& shaderName, ShaderStage shaderStage)
    {
        const auto& res = AssetsManager::Shaders.emplace(shaderPath, Shader{shaderName.data(), shaderStage});

        if (!res.second)
        {
            Log::Error("AssetsManager", "Tried to save a shader with an already used name");
            return nullptr;
        }

        return &(*res.first).second;
    }

    TextureImage* AssetsManager::GetTexImageByName(const std::string& assetName)
    {
        return &TextureImages[assetName];
    }

    ShaderPipeline AssetsManager::GetPipelinePBR()
    {
        return ShaderPipeline{ &PBR_VertexShader, nullptr, nullptr, nullptr, &PBR_FragmentShader };
    }

}
