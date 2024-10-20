
#include "AssetsManager.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "FileLoader.h"

#include <utility>
#include <iterator>
#include <Utils/Log.h>

namespace GaladHen
{
    // STATICS ------------------------------------------------------------------------------
    
    std::map<const std::string, Model> AssetsManager::Models = std::map<const std::string, Model>{};
    std::map<const std::string, Texture> AssetsManager::Textures = std::map<const std::string, Texture>{};
    std::map<const std::string, Shader> AssetsManager::Shaders = std::map<const std::string, Shader>{};

    Shader AssetsManager::PBR_VertexShader = Shader{ "../Shaders/pbr/Pbr.vert", ShaderStage::Vertex }; // default
    Shader AssetsManager::PBR_FragmentShader = Shader{ "../Shaders/pbr/Pbr.frag", ShaderStage::Fragment }; // default
    ShaderPipeline AssetsManager::PBR_ShaderPipeline = ShaderPipeline{ &PBR_VertexShader, nullptr, nullptr, nullptr, &PBR_FragmentShader };

    Shader AssetsManager::Unlit_VertexShader = Shader{ "../Shaders/Unlit/Unlit.vert", ShaderStage::Vertex };
    Shader AssetsManager::Unlit_FragmentShader = Shader{ "../Shaders/Unlit/Unlit.frag", ShaderStage::Fragment };
    ShaderPipeline AssetsManager::Unlit_ShaderPipeline = ShaderPipeline{ &Unlit_VertexShader, nullptr, nullptr, nullptr, &Unlit_FragmentShader };

    // STATICS ------------------------------------------------------------------------------

    void AssetsManager::FreeAssets()
    {
        AssetsManager::Models.clear();
        AssetsManager::Textures.clear();
        AssetsManager::Shaders.clear();
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

    Texture* AssetsManager::LoadAndStoreTexture(const std::string& texImgPath, const std::string& texImgName, TextureFormat textureFormat)
    {
        const auto& res = AssetsManager::Textures.emplace(texImgName, FileLoader::ReadImageFile(texImgPath.data(), textureFormat));

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

    Texture* AssetsManager::GetTextureByName(const std::string& assetName)
    {
        return &Textures[assetName];
    }

    Model* AssetsManager::StoreModel(const Model& source, const std::string& modelname)
    {
        const auto& res = Models.emplace(modelname, source);

        return &(*res.first).second;
    }

    ShaderPipeline* AssetsManager::GetPipelinePBR()
    {
        return &PBR_ShaderPipeline;
    }

    ShaderPipeline* AssetsManager::GetPipelineUnlit()
    {
        return &Unlit_ShaderPipeline;
    }

}
