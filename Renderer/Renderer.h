
// The Renderer is the heart of GaladHen, where:
// - api specific settings are set, api specific data are stored
// - the actual rendering happens

#pragma once

#include <vector>

namespace GaladHen
{
    class IMeshAPI;
    class Mesh;
    class Window;

    enum API
    {
        OpenGL = 0
    };

    class Renderer
    {

        friend class Mesh;

    public:

        Renderer(API apiToUse);

        // @brief
        // Create the low level pair of the high level Window
        // void CreateLowLevelWindow(Window& window);

        // @brief
        // Create the low level pair of the high level mesh, assigning a mesh id -> this is needed to render the mesh
        void CreateMeshLowLevelPair(Mesh& mesh);

        // @brief
        // Destroy the low level pair of the high level mesh, freeing memory
        void DestroyMeshLowLevelPair(Mesh& mesh);

    protected:

        API CurrentAPI;

        std::vector<IMeshAPI> MeshAPI;

    };
}
