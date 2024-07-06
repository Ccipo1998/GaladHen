
// The Renderer is the heart of GaladHen, where:
// - api specific settings are set, api specific data are stored
// - the actual rendering happens

// The Renderer is final because it's not supposed to have derivates: all API-specific operations are managed by API-specific objects inside the renderer 

#pragma once

#include <vector>
#include <glm/glm.hpp>

#include <Renderer/LayerAPI/IRendererAPI.h>

namespace GaladHen
{
    class Mesh;

    enum class API
    {
        OpenGL = 0
    };

    class Renderer final
    {

        friend class Mesh;

    public:

        Renderer(API apiToUse);

        // @brief
        // Create the low level pair of the high level mesh, assigning a mesh id -> this is needed to render the mesh
        void CreateLowLevelMesh(Mesh& mesh);

        // @brief
        // Destroy the low level pair of the high level mesh, freeing memory
        void DestroyLowLevelMesh(unsigned int meshID);

        // @brief
        // Specify clear color for color buffers
        void SetColorBufferClearColor(const glm::vec4 color);

        // @brief
        // Enable or disable depth testing
        void EnableDepthTest(bool enable);

    protected:

        API CurrentAPI;

        // API-specific renderer for API-specific operations
        IRendererAPI* RendererAPI;

    };
}
