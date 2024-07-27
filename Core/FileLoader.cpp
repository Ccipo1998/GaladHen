
#include "FileLoader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "Model.h"
#include "Texture.h"

// assimp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glm/glm.hpp>

#include <Utils/Log.h>
#include <Utils/stb_image.h>

namespace GaladHen
{
    std::string FileLoader::ReadTextFile(const char* filePath)
    {
        // ensure ifstream object can throw exceptions
        std::ifstream file;
        file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            // Open file
            file.open(filePath);
            
            std::stringstream stream;
            // Read file's buffer contents into streams
            stream << file.rdbuf();
            // close file handlers
            file.close();
            // Convert stream into string
            return stream.str();
        }
        catch(const std::exception& e)
        {
            std::string description = std::string{filePath} + " not succesfully read";
            Log::Error("FileLoader", e.what());
            return std::string{};
        }
    }

    Model FileLoader::ReadModelFile(const char* filePath)
    {
        // useful informations of assimp logic used in importing: https://assimp-docs.readthedocs.io/en/latest/usage/use_the_lib.html

        Model modelToReturn{};

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace); // 0 to avoid post-processing operations?

        // error checking
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::string error_message = "Assimp failed to import correctly the file " + std::string(filePath);
            Log::Error("FileLoader", error_message);

            return modelToReturn;
        }

        ProcessAssimpNode(scene->mRootNode, scene, &modelToReturn);

        return modelToReturn;
    }

    Texture FileLoader::ReadImageFile(const char* filePath, TextureFormat textureFormat)
    {
        int width, height, channels;
        unsigned char* bytes = stbi_load(filePath, &width, &height, &channels, 0);

        if (bytes == nullptr)
        {
            std::string error;
            error.append("Unable to open file with path: ");
            error.append(filePath);
            Log::Error("FileLoader", error);
        }

        return Texture{ bytes, width, height, channels, textureFormat };
    }

    void FileLoader::ProcessAssimpNode(aiNode* node, const aiScene* scene, Model* model)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; // node contains only indices of meshes, that are inside the scene data structure
            ProcessMesh(mesh, model);
        }

        // TODO: materials

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessAssimpNode(node->mChildren[i], scene, model);
        }
    }

    void FileLoader::ProcessMesh(aiMesh* mesh, Model* model)
    {
        model->Meshes.emplace_back(Mesh());
        Mesh& newMesh = model->Meshes[model->Meshes.size() - 1];

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
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

            newMesh.Vertices.push_back(vertex);
        }

        // for each face of the mesh, we retrieve the indices of its vertices , and we store them in a vector data structure
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                newMesh.Indices.push_back(face.mIndices[j]);
        }
    }
}
