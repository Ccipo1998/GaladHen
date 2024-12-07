
// The Renderer is the heart of GaladHen, where:
// - api specific settings are set, api specific data are stored
// - the actual rendering happens
// 
// All API-specific operations are managed by API-specific objects inside the renderer 

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>

#include "Common.h"
#include <Core/Transform.h>

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
    class Camera;
    class SceneObject;
    class RenderBuffer;

    namespace Renderer
    {
        // @brief
        // Perform renderer's generic initialization operations.
        // It is needed before any other call to the renderer
        void Init(API apiToUse = API::OpenGL);

        // @brief
        // Create a gpu render buffer
        std::shared_ptr<RenderBuffer> CreateRenderBuffer(unsigned int width, unsigned int height);

        // @brief
        // Clear a render buffer with a specific color
        void ClearRenderBuffer(std::shared_ptr<RenderBuffer> renderBuffer, glm::vec4 clearColor);

        // @brief
        // Draw request for a full scene. Using scene's camera, models and default render buffer
        void Draw(const Scene& scene);

        // @brief
        // Update model data into gpu memory -> it is done only if the model is already in cache
        void UpdateModel(const Model& model);

        // @brief
        // Update model data into gpu memory -> it is done only if the model is already in cache
        void UpdateMesh(const Mesh& mesh);

        // @brief
        // Free model data from gpu memory
        void FreeModel(const Model& model);

        // @brief
        // Free mesh data from gpu memory
        void FreeMesh(const Mesh& mesh);



        // @brief
        // Compile all the shaders used by the scene objects in the scene
        // @returns: true if all the shaders compile successfully; false otherwise
        bool CompileShaders(Scene& scene);

        // @brief
        // Compile a shader pipeline
        // @returns: compilation result
        bool CompileShader(ShaderPipeline& shader);

        // @brief
        // Compile a compute shader
        // @returns: compilation result
        bool CompileShader(ComputeShader& shader);

        // @brief
        // Free shader pipeline code resources from gpu memory
        void FreeShader(ShaderPipeline& shader);

        // @brief
        // Free compute shader code resources from gpu memory
        void FreeShader(ComputeShader& shader);

        // @brief
        // Update texture data into gpu memory -> it is done only if the texture is already in cache
        void UpdateTexture(Texture& texture);

        // @brief
        // Free texture data from gpu memory
        void FreeTexture(Texture& texture);

        // brief
        // Load all lighting data used inside the scene
        void LoadLightingData(Scene& scene);

        // brief
        // Update all lighting data used inside the scene
        // LoadLightingData() must be already called
        void UpdateLightingData(Scene& scene);

        // @brief
        // Free alla lighting data used by the scene
        void FreeLightingData(Scene& scene);

        // @brief
        // Returns the default render buffer object
        const std::shared_ptr<RenderBuffer> GetDefaultRenderBuffer();

        //void SetViewport(const glm::uvec2& position, const glm::uvec2& size);

        //void SetRenderTargetSize(const glm::uvec2& size);
    };
}
