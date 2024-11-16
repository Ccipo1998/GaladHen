
// The Renderer is the heart of GaladHen, where:
// - api specific settings are set, api specific data are stored
// - the actual rendering happens

// The Renderer is final because it's not supposed to have derivates: all API-specific operations are managed by API-specific objects inside the renderer 

#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Common.h"
#include "LayerAPI/IRendererAPI.h"

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

    class Renderer final
    {
    public:

        Renderer();

        // @brief
        // Perform generic initialization operations
        void Init(API apiToUse = API::OpenGL);

        // @brief
        // Operations needed before draw calls
        void BeginDraw();

        // @brief
        // Operations needed after draw calls
        void EndDraw();

        // @brief
        // Load all the models used by the scene
        void LoadModels(Scene& scene);

        // @brief
        // Free memory of all models used by the scene
        void FreeModels(Scene& scene);

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
        // Free texture data from GPU memory
        void FreeTexture(Texture& texture);

        // @brief
        // Load material data into GPU memory
        void LoadMaterialData(Material& material);

        // @brief
        // Load camera data (position, orientation, ...) into GPU memory
        void LoadCameraData(Camera& camera);

        // @brief
        // Update camera data (position, orientation, ...) into GPU memory
        // LoadCameraData() must be already called
        void UpdateCameraData(Camera& camera);

        // @brief
        // Load transform data (position, orientation and scale) into GPU memory
        void LoadTransformData();

        // @brief
        // Update transform data (position, orientation and scale) into GPU memory
        // LoadTransformData() must be already called
        void UpdateTransformData(TransformQuat& transform);

        // @brief
        // Draw calls on each scene object of the scene
        void Draw(Scene& scene);

        // @brief
        // Drawcall for a mesh with a material
        void Draw(Mesh& mesh, Material& material);

        // @brief
        // Drawcall for a scene object
        void Draw(SceneObject& object);

        // @brief
        // Compile a shader pipeline program
        // @returns: compilation result
        bool CompileShaderPipeline(ShaderPipeline& program);

        // @brief
        // Compile a compute shader program
        // @returns: compilation result
        bool CompileComputeShader(ComputeShader& program);

        // @brief
        // Returns the current render buffer object
        const RenderBuffer GetRenderBuffer() const;

        void SetViewport(const glm::uvec2& position, const glm::uvec2& size);

        void SetRenderTargetSize(const glm::uvec2& size);

    protected:

        API CurrentAPI;

        // API-specific renderer for API-specific operations
        IRendererAPI* RendererAPI;

    };
}
