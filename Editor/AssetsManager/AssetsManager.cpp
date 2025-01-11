
#include "AssetsManager.h"

#include <unordered_map>
#include <utility>
#include <iterator>
#include <Utils/Log.h>
#include <memory>

#include <Renderer/Entities/Model.h>
#include <Renderer/Entities/Texture.h>
#include <Renderer/Entities/ShaderPipeline.h>
#include "AssetLoader.h"

namespace GaladHen
{
    namespace AssetsManager
    {
        namespace
        {
            std::unordered_map<std::string, std::shared_ptr<Model>> Models;
            std::unordered_map<std::string, std::shared_ptr<Texture>> Textures;
            std::unordered_map<std::string, std::shared_ptr<ShaderPipeline>> ShaderPipelines;
        }

        std::shared_ptr<Model> LoadAndStoreModel(const std::string& modelPath, const std::string& modelName)
        {
            if (Models.find(modelPath) != Models.end())
            {
                Log::Warning("AssetsManager", "Tried to save a model with an already used name");
                return nullptr;
            }

            Model* model = AssetLoader::LoadModelFromFile(modelPath.data());
            std::shared_ptr<Model> outModel = std::shared_ptr<Model>{ model };
            if (model == nullptr)
            {
                Log::Warning("AssetsManager", "Failed to load model");
                return nullptr;
            }

            Models.emplace(modelName, outModel);

            return outModel;
        }

        std::shared_ptr<Texture> LoadAndStoreTexture(const std::string& texturePath, const std::string& textureName, TextureFormat textureFormat)
        {
            if (Textures.find(textureName) != Textures.end())
            {
                Log::Warning("AssetsManager", "Tried to save a texture image with an already used name");
                return nullptr;
            }

            Texture* texture = AssetLoader::LoadTextureFromFile(texturePath.data(), textureFormat);
            std::shared_ptr<Texture> outTexture = std::shared_ptr<Texture>{ texture };
            if (texture == nullptr)
            {
                Log::Warning("AssetsManager", "Failed to load texture");
                return nullptr;
            }

            Textures.emplace(textureName, outTexture);

            return outTexture;
        }

        std::shared_ptr<ShaderPipeline> LoadAndStoreShaderPipeline(
            const std::string& vShaderPath,
            const std::string& tContShaderPath,
            const std::string& tEvalShaderPath,
            const std::string& gShaderPath,
            const std::string& fShaderPath,
            const std::string& cShaderPath,
            const std::string& pipelineName)
        {
            if (ShaderPipelines.find(pipelineName) != ShaderPipelines.end())
            {
                Log::Warning("AssetsManager", "Tried to save a shader pipeline with an already used name");
                return nullptr;
            }

            ShaderPipeline* pipeline;
            if (cShaderPath.length() > 0)
            {
                pipeline = new ShaderPipeline{ cShaderPath };
            }
            else
            {
                pipeline = new ShaderPipeline{ vShaderPath, tContShaderPath, tEvalShaderPath, gShaderPath, fShaderPath };
            }

            std::shared_ptr<ShaderPipeline> outPipeline = std::shared_ptr<ShaderPipeline>{ pipeline };
            ShaderPipelines.emplace(pipelineName, outPipeline);

            return outPipeline;
        }

        std::shared_ptr<Model> AssetsManager::GetModelByName(const std::string& assetName)
        {
            return Models[assetName];
        }

        std::shared_ptr<Texture> AssetsManager::GetTextureByName(const std::string& assetName)
        {
            return Textures[assetName];
        }

        std::shared_ptr<ShaderPipeline> GetShaderPipelineByName(const std::string& pipelineName)
        {
            return ShaderPipelines[pipelineName];
        }

        std::shared_ptr<Model> AssetsManager::StoreModel(const Model& model, const std::string& modelName)
        {
            if (Models.find(modelName) != Models.end())
            {
                Log::Warning("AssetsManager", "Tried to save a model with an already used name");
                return nullptr;
            }

            Model* storedModel = new Model{ model };
            std::shared_ptr<Model> outModel = std::shared_ptr<Model>{ storedModel };
            Models.emplace(modelName, outModel);

            return outModel;
        }

        void FreeAssets()
        {
            Models.clear();
            Textures.clear();
            ShaderPipelines.clear();
        }
    }
}