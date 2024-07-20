
#pragma once

#include <string>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace GaladHen
{
    class Model;
    class TextureImage;

    class FileLoader
    {
    public:

        static std::string ReadTextFile(const char* filePath);

        static Model ReadModelFile(const char* filePath);

        static TextureImage ReadImageFile(const char* filePath);

    protected:

        // @brief
        // Util function for assimp model loading
        static void ProcessAssimpNode(aiNode* node, const aiScene* scene, Model* model);

        // @brief
        // Convert assimp data structure into GaladHen data structure
        static void ProcessMesh(aiMesh* mesh, Model* model);
    };
}
