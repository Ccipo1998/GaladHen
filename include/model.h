/*
Model class

Model and Mesh classes follow RAII principles (https://en.cppreference.com/w/cpp/language/raii).
Model is a "move-only" class. A move-only class ensures that you always have a 1:1 relationship between the total number of resources being created and the total number of actual instantiations occurring.

*/

#include <vector>
#include <iostream>
#include "mesh.h"

// assimp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

using namespace std;
using namespace glm;

class Model
{
    public:

    vector<Mesh> Meshes;
    // TODO: vector<materials>

    // TODO: position and rotation or a matrix

    // We want Model to be a move-only class. We delete copy constructor and copy assignment
    // see:
    // https://docs.microsoft.com/en-us/cpp/cpp/constructors-cpp?view=vs-2019
    // https://en.cppreference.com/w/cpp/language/copy_constructor
    // https://en.cppreference.com/w/cpp/language/copy_assignment
    // https://www.geeksforgeeks.org/preventing-object-copy-in-cpp-3-different-ways/
    // Section 4.6 of the "A Tour in C++" book
    Model(const Model& model) = delete; //disallow copy
    Model& operator=(const Model& copy) = delete;

    // For the Model class, a default move constructor and move assignment is sufficient
    // see:
    // https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=vs-2019
    // https://en.cppreference.com/w/cpp/language/move_constructor
    // https://en.cppreference.com/w/cpp/language/move_assignment
    // https://www.learncpp.com/cpp-tutorial/15-1-intro-to-smart-pointers-move-semantics/
    // https://www.learncpp.com/cpp-tutorial/15-3-move-constructors-and-move-assignment/
    // Section 4.6 of the "A Tour in C++" book
    Model& operator=(Model&& move) noexcept = default;
    Model(Model&& model) = default; //internally does a memberwise std::move

    Model(const char* modelPath)
    {
        // file model loading using assimp
        this->loadModelFile(modelPath);
    }

    void draw()
    {
        for (GLuint i = 0; i < Meshes.size(); i++)
        {
            Meshes[i].draw();
        }
    }

    private:

    // load 3D model from a file using assimp
    void loadModelFile(const char* modelPath)
    {
        // useful informations of assimp logic used in importing: https://assimp-docs.readthedocs.io/en/latest/usage/use_the_lib.html

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace); // 0 to avoid post-processing operations?

        // error checking
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "Error: Assimp failed to import correctly the file " << modelPath << endl; // TODO: use a global printError function (like the one in shader.h) to write many types of errors (at least with the same format)

            return; // void model is created
        }

        this->processNode(scene->mRootNode, scene);
    }

    // process of each node of assimp's data structure
    void processNode(aiNode* node, const aiScene* scene)
    {
        for (GLuint i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; // node contains only indices of meshes, that are inside the scene data structure
            this->Meshes.emplace_back(this->processMesh(mesh)); // emplace_back instead of push_back -> the object to place in the vector is directly stored there, without a temporary value
        }

        // TODO: materials

        for(GLuint i = 0; i < node->mNumChildren; i++)
        {
            this->processNode(node->mChildren[i], scene);
        }
    }

    // process assimp data structure for mesh into ezengine data structure for mesh
    Mesh processMesh(aiMesh* mesh)
    {
        vector<Vertex> vertices;
        vector<GLuint> indices;

        for(GLuint i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            vec3 vector;
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
                vec2 vec;
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
                vertex.UV = vec2(0.0f, 0.0f);
                vertex.Tangent = vec3(0.0f, 0.0f, 0.0f);
                vertex.Bitangent = vec3(0.0f, 0.0f, 0.0f);
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
};
