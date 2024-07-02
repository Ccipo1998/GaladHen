
// Model class representing a collection of meshes

#pragma once

#include <string>
#include <vector>

#include "Mesh.h"

struct aiNode;
struct aiScene;
struct aiMesh;

namespace GaladHen
{
    class Model
    {

    public:

        Model();

        // Copy constructor
        Model(const Model& model) noexcept;
        // Copy assignment
        Model& operator=(const Model& model) noexcept;
        // Move assignment
        Model& operator=(Model&& model) noexcept;
        // Move constructor
        Model(Model&& model) noexcept;

        // @brief
        // Model creation by file loading
        Model(const std::string& modelPath);

        std::vector<Mesh> Meshes;

    protected:

        // @brief
        // Load 3D model from a file using assimp
        void LoadModelFile(const std::string& modelPath);

        // @brief
        // Util function for assimp model loading
        void ProcessAssimpNode(aiNode* node, const aiScene* scene);

        // @brief
        // Convert assimp data structure into GaladHen data structure
        void ProcessMesh(aiMesh* mesh);
    };
}
