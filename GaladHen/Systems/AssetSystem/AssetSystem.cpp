
#include "AssetSystem.h"

#include <Utils/Log.h>
// assimp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <Utils/stb_image.h>

namespace GaladHen
{
	AssetSystem::AssetSystem()
	{}

    std::weak_ptr<Model> AssetSystem::LoadAndStoreModel(const std::string& modelPath, const std::string& modelName)
	{
        if (Models.find(modelPath) != Models.end())
        {
            Log::Warning("AssetSystem", "Tried to save a model with an already used name");
            return std::weak_ptr<Model>{};
        }

        Model* model = LoadModelFromFile(modelPath.data());
        if (model == nullptr)
        {
            Log::Warning("AssetSystem", "Failed to load model");
            return std::weak_ptr<Model>{};
        }

        std::shared_ptr<Model> uniqueModel = std::shared_ptr<Model>{ model };
        Models.emplace(modelName, uniqueModel);

        return std::weak_ptr<Model>{ uniqueModel };
	}

    std::weak_ptr<Model> AssetSystem::CreateAndStoreModel(const std::string& modelName)
    {
        Model* model = new Model{};
        std::shared_ptr<Model> uniqueModel = std::shared_ptr<Model>{ model };
        Models.emplace(modelName, uniqueModel);

        return std::weak_ptr<Model>{ uniqueModel };
    }

    std::weak_ptr<Texture> AssetSystem::LoadAndStoreTexture(const std::string& texturePath, const std::string& textureName, TextureFormat textureFormat)
    {
        if (Textures.find(textureName) != Textures.end())
        {
            Log::Warning("AssetSystem", "Tried to save a texture image with an already used name");
            return std::weak_ptr<Texture>{};
        }

        Texture* texture = LoadTextureFromFile(texturePath.data(), textureFormat);
        if (texture == nullptr)
        {
            Log::Warning("AssetSystem", "Failed to load texture");
            return std::weak_ptr<Texture>{};
        }

        std::shared_ptr<Texture> uniqueTexture = std::shared_ptr<Texture>{ texture };
        Textures.emplace(textureName, uniqueTexture);

        return std::weak_ptr<Texture>{ uniqueTexture };
    }

    std::weak_ptr<ShaderPipeline> AssetSystem::LoadAndStoreShaderPipeline(const std::string& vShaderPath, const std::string& tContShaderPath, const std::string& tEvalShaderPath, const std::string& gShaderPath, const std::string& fShaderPath, const std::string& cShaderPath, const std::string& pipelineName)
    {
        if (ShaderPipelines.find(pipelineName) != ShaderPipelines.end())
        {
            Log::Warning("AssetSystem", "Tried to save a shader pipeline with an already used name");
            return std::weak_ptr<ShaderPipeline>{};
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

        std::shared_ptr<ShaderPipeline> uniquePipeline = std::shared_ptr<ShaderPipeline>{ pipeline };
        ShaderPipelines.emplace(pipelineName, uniquePipeline);

        return std::weak_ptr<ShaderPipeline>{ uniquePipeline };
    }

    std::weak_ptr<Model> AssetSystem::GetModelByName(const std::string& modelName)
    {
        auto modelIt = Models.find(modelName);
        if (modelIt != Models.end())
        {
            return std::weak_ptr<Model>{ (*modelIt).second };
        }

        return std::weak_ptr<Model>{};
    }

    std::weak_ptr<Texture> AssetSystem::GetTextureByName(const std::string& textureName)
    {
        auto texturelIt = Textures.find(textureName);
        if (texturelIt != Textures.end())
        {
            return std::weak_ptr<Texture>{ (*texturelIt).second };
        }

        return std::weak_ptr<Texture>{};
    }

    std::weak_ptr<ShaderPipeline> AssetSystem::GetShaderPipelineByName(const std::string& pipelineName)
    {
        auto pipelinelIt = ShaderPipelines.find(pipelineName);
        if (pipelinelIt != ShaderPipelines.end())
        {
            return std::weak_ptr<ShaderPipeline>{ (*pipelinelIt).second };
        }

        return std::weak_ptr<ShaderPipeline>{};
    }

    std::weak_ptr<Material> AssetSystem::CreateAndStoreMaterial(const std::string& materialName)
    {
        Material* material = new Material{};
        std::shared_ptr<Material> uniqueMaterial = std::shared_ptr<Material>{ material };
        Materials.emplace(materialName, uniqueMaterial);

        return std::weak_ptr<Material>{ uniqueMaterial };
    }

    void AssetSystem::Init()
    {

    }

    void AssetSystem::Quit()
    {
        // Free resources

        Models.clear();
        Textures.clear();
        ShaderPipelines.clear();
        Materials.clear();
    }

    Model* AssetSystem::LoadModelFromFile(const char* filePath)
    {
        // useful informations of assimp logic used in importing: https://assimp-docs.readthedocs.io/en/latest/usage/use_the_lib.html

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace); // 0 to avoid post-processing operations?

        // error checking
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::string error_message = "Assimp failed to import correctly the file " + std::string(filePath);
            Log::Error("AssetSystem", error_message);

            return nullptr;
        }

        Model* model = new Model{};
        ProcessAssimpNode(scene->mRootNode, scene, model->Meshes);

        return model;
    }

    Texture* AssetSystem::LoadTextureFromFile(const char* filePath, TextureFormat textureFormat)
    {
        // TODO: trovare modo di poter leggere il formato della texture e/o fare check su numero canali trovati dalla stbi_load e il parametro texture format

        int width, height, channels;
        unsigned char* bytes = stbi_load(filePath, &width, &height, &channels, 0);

        if (bytes == nullptr)
        {
            std::string error;
            error.append("Unable to open file with path: ");
            error.append(filePath);
            Log::Error("FileLoader", error);

            return nullptr;
        }

        return new Texture{ bytes, (unsigned int)width, (unsigned int)height, 0, textureFormat };
    }

    void AssetSystem::ProcessMesh(aiMesh* mesh, std::vector<Mesh>& outMeshes)
    {
        std::vector<MeshVertexData> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            MeshVertexData vertex;
            glm::vec3 vector;
            // vertices coordinates
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // Normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            // Texture Coordinates
            // if the model has texture coordinates, than we assign them to a GLM data structure, otherwise we set them at 0
            // if texture coordinates are present, than Assimp can calculate tangents and bitangents, otherwise we set them at 0 too
            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                // in this example we assume the model has only one set of texture coordinates. Actually, a vertex can have up to 8 different texture coordinates. For other models and formats, this code needs to be adapted and modified.
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.UV = vec;

                // Tangents
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // Bitangents
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
            {
                vertex.UV = glm::vec2(0.0f, 0.0f);
                vertex.Tangent = glm::vec3(0.0f, 0.0f, 0.0f);
                vertex.Bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        // for each face of the mesh, we retrieve the indices of its vertices , and we store them in a vector data structure
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        outMeshes.emplace_back(Mesh{ vertices, indices, MeshPrimitive::Triangle });
    }

    void AssetSystem::ProcessAssimpNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& outMeshes)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; // node contains only indices of meshes, that are inside the scene data structure
            ProcessMesh(mesh, outMeshes);
        }

        // TODO: materials

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessAssimpNode(node->mChildren[i], scene, outMeshes);
        }
    }
}
