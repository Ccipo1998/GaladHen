
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
        // Get default front render buffer
        std::shared_ptr<RenderBuffer> GetFrontRenderBuffer();

        // @brief
        // Get the low-level api id for a render buffer's texture
        unsigned int GetRenderBufferColorApiID(std::shared_ptr<RenderBuffer> renderBuffer);

        // @brief
        // Clear a render buffer with a specific color
        void ClearRenderBuffer(std::shared_ptr<RenderBuffer> renderBuffer);

        // @brief
        // Draw request for a full scene. Using scene's camera, models and default render buffer
        void Draw(Scene& scene);

        // @brief
        // Draw request for a full scene. Using scene's camera and models, using specific render buffer
        void Draw(Scene& scene, std::shared_ptr<RenderBuffer> renderBuffer);

        // @brief
        // Update model data into gpu memory -> it is done only if the model is already in cache
        //void UpdateModel(const Model& model);

        // @brief
        // Update model data into gpu memory -> it is done only if the model is already in cache
        //void UpdateMesh(const Mesh& mesh);

        // @brief
        // Compile all the shaders used by the scene objects in the scene
        // @returns: true if all the shaders compile successfully; false otherwise
        bool CompileShaders(const Scene& scene);

        // @brief
        // Compile a shader pipeline
        // @returns: compilation result
        bool CompileShader(ShaderPipeline& shader);

        // @brief
        // Update texture data into gpu memory -> it is done only if the texture is already in cache
        //void UpdateTexture(Texture& texture);
    };
}
