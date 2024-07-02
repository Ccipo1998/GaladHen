
// The Renderer is the heart of GaladHen, where:
// - api specific settings are set, api specific data are stored
// - the actual rendering happens

// The Renderer is final because it's not supposed to have derivates: all API-specific operations are managed by API-specific objects inside the renderer 

#pragma once

#include <vector>
#include <glm/glm.hpp>

#include <Renderer/LayerAPI/IMeshAPI.h>

namespace GaladHen
{
    class Mesh;
    class Window;

    enum API
    {
        OpenGL = 0
    };

    class Renderer final
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
        void DestroyMeshLowLevelPair(unsigned int meshID);

        // @brief
        // Specify clear color for color buffers
        void SetColorBufferClearColor(const glm::vec4 color);

        // @brief
        // Enable or disable depth testing
        void EnableDepthTest(bool enable);

    protected:

        API CurrentAPI;

        std::vector<IMeshAPI> MeshAPI;

    };
}
