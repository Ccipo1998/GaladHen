
#pragma once

#include <Systems/SystemsCoordinator.h>
#include <Systems/ISystem.h>
#include <Utils/WeakSingleton.hpp>
#include <unordered_map>
#include <string>
#include <Systems/RenderingSystem/Entities/Model.h>
#include <Systems/RenderingSystem/Entities/Texture.h>
#include <Systems/RenderingSystem/Entities/ShaderPipeline.h>
#include <Systems/RenderingSystem/Entities/Material.h>
#include <memory>

struct aiMesh;
struct aiNode;
struct aiScene;

namespace GaladHen
{
	class AssetSystem : public ISystem, public WeakSingleton<AssetSystem>
	{
		friend SystemsCoordinator;

	public:

		AssetSystem();

		// @brief
		// Load a model from a file and make it owned by asset system
		std::weak_ptr<Model> LoadAndStoreModel(const std::string& modelPath, const std::string& modelName);

		// @brief
		// Create a new model and make it owned by asset system
		std::weak_ptr<Model> CreateAndStoreModel(const std::string& modelName);

		// @brief
		// Load a texture from a file and make it owned by asset system
		std::weak_ptr<Texture> LoadAndStoreTexture(const std::string& texturePath, const std::string& textureName, TextureFormat textureFormat);

		// @brief
		// Load a shader pipeline from a shader files and make it owned by asset system
		std::weak_ptr<ShaderPipeline> LoadAndStoreShaderPipeline(
			const std::string& vShaderPath,
			const std::string& tContShaderPath,
			const std::string& tEvalShaderPath,
			const std::string& gShaderPath,
			const std::string& fShaderPath,
			const std::string& cShaderPath,
			const std::string& pipelineName);

		// @brief
		// Get a reference to a model owned by asset system by its name
		std::weak_ptr<Model> GetModelByName(const std::string& modelName);

		// @brief
		// Get a reference to a texture owned by asset system by its name
		std::weak_ptr<Texture> GetTextureByName(const std::string& textureName);

		// @brief
		// Get a reference to a shader pipeline owned by asset system by its name
		std::weak_ptr<ShaderPipeline> GetShaderPipelineByName(const std::string& pipelineName);

		// @brief
		// Create a new material and make it owned by asset system
		std::weak_ptr<Material> CreateAndStoreMaterial(const std::string& materialName);

	private:

		virtual void Init() override;

		virtual void Quit() override;

		Model* LoadModelFromFile(const char* filePath);
		Texture* LoadTextureFromFile(const char* filePath, TextureFormat textureFormat);

		void ProcessMesh(aiMesh* mesh, std::vector<Mesh>& outMeshes);
		void ProcessAssimpNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& outMeshes);

		std::unordered_map<std::string, std::shared_ptr<Model>> Models;
		std::unordered_map<std::string, std::shared_ptr<Texture>> Textures;
		std::unordered_map<std::string, std::shared_ptr<ShaderPipeline>> ShaderPipelines;
		std::unordered_map<std::string, std::shared_ptr<Material>> Materials;
	};
}
