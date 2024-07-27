
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
    class Model;
    class Scene;
    class SceneStatus;
    class ShaderPipeline;
    class ComputeShader;
    class Material;
    class Texture;

    class Renderer final
    {
    public:

        Renderer(API apiToUse);

        // @brief
        // Perform generic initialization operations
        void Init();

        // @brief
        // Load all the models used by the scene
        void LoadModels(Scene& scene);

        // @brief
        // Free memory of all models used by the scene
        void FreeModels(Scene& scene);

        // brief
        // Load all lighting data used inside the scene
        void LoadLightingData(Scene& scene);

        // @brief
        // Free alla lighting data used by the scene
        void FreeLightingData(Scene& scene);

        // @brief
        // Compile all the shaders used by the scene
        void CompileShaders(Scene& scene);

        // @brief
        // Load of mesh data into gpu memory
        void LoadMesh(Mesh& mesh);

        // @brief
        // Free gpu memory from mesh data, plus clearing internal data
        void FreeMesh(Mesh& mesh);

        // @brief
        // Load of model data into gpu memory
        void LoadModel(Model& model);

        // @brief
        // Free gpu memory from model data, plus clearing internal data
        void FreeModel(Model& model);

        // @brief
        // Load texture data into GPU memory
        void LoadTexture(Texture& texture);

        // @brief
        // Load material data into GPU memory
        void LoadMaterialData(Material& material);

        // @brief
        // Draw calls on each scene object of the scene
        void Draw(Scene& scene);

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
