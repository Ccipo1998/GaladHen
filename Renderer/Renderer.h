
// The Renderer is the heart of GaladHen, where:
// - api specific settings are set, api specific data are stored
// - the actual rendering happens

// The Renderer is final because it's not supposed to have derivates: all API-specific operations are managed by API-specific objects inside the renderer 

#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Common.h"
#include <Renderer/LayerAPI/IRendererAPI.h>

namespace GaladHen
{
    class Mesh;

    class Renderer final
    {

        friend class Mesh;

    public:

        Renderer(API apiToUse);

        // @brief
        // Perform initialization operations
        void Init();

        // @brief
        // Load mesh data into gpu memory, plus assigning a mesh id for internal use
        void SendMeshDataToGPU(Mesh& mesh);

        // @brief
        // Free gpu memory from mesh data, plus clearing internal data
        void FreeMeshDataFromGPU(Mesh& mesh);

        // @brief
        // Enable or disable depth testing
        void EnableDepthTest(bool enable);

    protected:

        API CurrentAPI;

        // API-specific renderer for API-specific operations
        IRendererAPI* RendererAPI;

    };
}
