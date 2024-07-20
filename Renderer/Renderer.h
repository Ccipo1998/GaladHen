
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
    class Scene;
    class SceneStatus;
    class ShaderPipeline;
    class ComputeShader;

    class Renderer final
    {

        friend class Mesh;

    public:

        Renderer(API apiToUse);

        // @brief
        // Perform generic initialization operations
        void Init();

        // @brief
        // Perform initialization operations needed from the renderer to render the scene
        void InitScene(Scene& scene);

        // @brief
        // Update renderer data basing on updated scene data
        void UpdateFromScene(Scene& scene, SceneStatus& status);

        // @brief
        // Draw calls on each scene object of the scene
        void Draw(Scene& scene);

        // @brief
        // Clear memory of all objects and data inside the scene
        void ClearScene(Scene& scene);

        // @brief
        // First load or update of mesh data into gpu memory, basing on mesh id
        void LoadMeshDataIntoGPU(Mesh& mesh);

        // @brief
        // Free gpu memory from mesh data, plus clearing internal data
        void FreeMeshDataFromGPU(Mesh& mesh);

        // @brief
        // Compile a shader pipeline program
        // @returns: compilation result
        bool CompileShaderPipeline(ShaderPipeline& program);

        // @brief
        // Compile a compute shader program
        // @returns: compilation result
        bool CompileComputeShader(ComputeShader& program);

        // @brief
        // Enable or disable depth testing
        void EnableDepthTest(bool enable);

    protected:

        API CurrentAPI;

        // API-specific renderer for API-specific operations
        IRendererAPI* RendererAPI;

    };
}
