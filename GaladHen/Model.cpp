
#include "Model.h"
#include "Mesh.h"

#include <iostream>

// assimp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <Utils/Log.h>

namespace GaladHen
{
    Model::Model() {}

    Model::Model(const Model& model) noexcept
        : Meshes(model.Meshes)
        {}

    Model& Model::operator=(const Model& model) noexcept
    {
        Meshes = model.Meshes;

        return *this;
    }

    Model& Model::operator=(Model&& model) noexcept
    {
        Meshes = std::move(model.Meshes);

        return *this;
    }

    Model::Model(Model&& model) noexcept
        : Meshes(std::move(model.Meshes))
        {}

    Model::Model(const std::string& modelPath)
    {
        // file model loading using assimp
        LoadModelFile(modelPath);
    }

    void Model::LoadModelFile(const std::string& modelPath)
    {
        // useful informations of assimp logic used in importing: https://assimp-docs.readthedocs.io/en/latest/usage/use_the_lib.html

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace); // 0 to avoid post-processing operations?

        // error checking
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::string error_message = "Assimp failed to import correctly the file " + std::string(modelPath);
            Log::Error("Model", error_message);

            return; // void model is created
        }

        ProcessAssimpNode(scene->mRootNode, scene);
    }

    void Model::ProcessAssimpNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; // node contains only indices of meshes, that are inside the scene data structure
            ProcessMesh(mesh);
        }

        // TODO: materials

        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessAssimpNode(node->mChildren[i], scene);
        }
    }

    void Model::ProcessMesh(aiMesh* mesh)
    {
        Meshes.emplace_back(Mesh());
        Mesh& newMesh = Meshes[Meshes.size() - 1];

        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
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
            if(mesh->mTextureCoords[0])
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

            newMesh.Vertices.push_back(vertex);
        }

        // for each face of the mesh, we retrieve the indices of its vertices , and we store them in a vector data structure
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                newMesh.Indices.push_back(face.mIndices[j]);
        }
    }

}

