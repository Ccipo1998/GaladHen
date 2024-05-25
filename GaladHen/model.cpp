
#include "model.h"
#include "mesh.h"

#include <iostream>

// assimp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <Utils/log.h>

Model::Model() {}

Model::Model(const std::string& modelPath)
{
    // file model loading using assimp
    this->LoadModelFile(modelPath);
}

void Model::Draw() const
{
    for (GLuint i = 0; i < Meshes.size(); i++)
    {
        Meshes[i].Draw();
    }
}

void Model::Draw(const std::vector<Material*>& materials) const
{
    if (this->Meshes.size() != materials.size())
    {
        Log::Error("Model", "Incompatible number of meshes and materials before the drawcall");

        return;
    }

    for (GLuint i = 0; i < Meshes.size(); i++)
    {
        Meshes[i].Draw(materials[i]);
    }
}

void Model::Draw(const std::vector<Material*>&& materials) const
{
    if (this->Meshes.size() != materials.size())
    {
        Log::Error("Model", "Incompatible number of meshes and materials before the drawcall");

        return;
    }

    for (GLuint i = 0; i < Meshes.size(); i++)
    {
        Meshes[i].Draw(materials[i]);
    }
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

    this->ProcessAssimpNode(scene->mRootNode, scene);
}

void Model::ProcessAssimpNode(aiNode* node, const aiScene* scene)
{
    for (GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; // node contains only indices of meshes, that are inside the scene data structure
        this->Meshes.emplace_back(this->ProcessMesh(mesh)); // emplace_back instead of push_back -> the object to place in the vector is directly stored there, without a temporary value
    }

    // TODO: materials

    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
        this->ProcessAssimpNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for(GLuint i = 0; i < mesh->mNumVertices; i++)
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

        vertices.push_back(vertex);
    }

    // for each face of the mesh, we retrieve the indices of its vertices , and we store them in a vector data structure
    for(GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return Mesh(vertices, indices);
}
