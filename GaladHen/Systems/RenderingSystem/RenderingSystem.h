
#pragma once

#include <memory>
#include <unordered_set>

#include <Systems/ISystem.h>
#include "LayerAPI/IRendererAPI.h"
#include <Utils/WeakSingleton.hpp>
#include <Systems/SystemsCoordinator.h>
#include "Common.h"
#include "Entities/Camera.h"
#include <Utils/IdList.hpp>
#include "Entities/Buffer.hpp"
#include "Entities/BufferData/CameraBufferData.h"
#include "Entities/BufferData/TransformBufferData.h"
#include "Entities/BufferData/LightingBufferData.h"
#include "Entities/BufferData/PointLightBufferData.h"
#include "Entities/BufferData/DirLightBufferData.h"
#include "UI/Page.h"
#include <type_traits>

namespace GaladHen
{
    class RenderBuffer;
    class ShaderPipeline;
    class Scene;
    class Mesh;
    class Material;
    class Texture;
    class PointLight;
    class DirectionalLight;

	class RenderingSystem : public ISystem, public WeakSingleton<RenderingSystem>
	{
        friend SystemsCoordinator;

	public:

        RenderingSystem();

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
        // Draw request for current UIPage
        void DrawUI();

        // @brief
        // Set current UIPage
        template <class T>
        void SetUIPage(const std::string& pageName)
        {
            static_assert(std::is_base_of<UIPage, T>::value, "Template class must inherit from UIPage class");

            if (CurrentUIPage)
                delete CurrentUIPage;

            CurrentUIPage = new T{ pageName.data()};
        }

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

        void CreateMainWindow(const std::string& name, glm::uvec2 size);

        void CloseMainWindow();

        // @brief
        // Update texture data into gpu memory -> it is done only if the texture is already in cache
        //void UpdateTexture(Texture& texture);

	private:

        virtual void Init() override;

        virtual void Quit() override;

		// INTERNAL CLASSES -----------------------------------------------------------------

		enum class RenderContextType
		{
			SingleBuffering,
			DoubleBuffering
		};

        // A render context is a set of data for a series of related draw calls (e.g. same render buffer, same camera, ...)
        class RenderContext
        {
        public:
            // TODO: RenderScene -> collection of models and theirs Bounding Volumes + spatial partitioning structure containing all the models (for frustum culling)

            RenderContext(RenderingSystem* renderingSys, unsigned int width, unsigned int height, RenderContextType renderContextType);

            RenderContextType GetRenderContextType();

            std::shared_ptr<RenderBuffer> GetFrontBuffer();

            std::shared_ptr<RenderBuffer> GetBackBuffer();

            void SwapBuffers();

            Camera RenderingCamera;

        private:

            RenderContextType RenderContextType;
            // RenderBuffer(s) -> two in case of multiple buffering
            std::shared_ptr<RenderBuffer> FrontBuffer;
            std::shared_ptr<RenderBuffer> BackBuffer;

        };

		// RENDERER DATA --------------------------------------------------------------------

		API CurrentAPI;
		std::unique_ptr<IRendererAPI> RendererAPI; // API-specific renderer for API-specific operations
		bool Initialized;

        // Rendering data
        IdList<RenderContext> RenderContexts; // the RenderContext with id 1 is the default one
        std::unordered_set<unsigned int> LoadedMeshesCache; // cache of already loaded meshes -> for reloading, it requires that a mesh knows when it has been modified
        std::unordered_set<unsigned int> LoadedTexturesCache; // cache of already loaded textures -> for reloading, it requires that a texture knows when it has been modified
        std::unordered_set<unsigned int> LoadedBuffersCache; // cache of already loaded buffers -> for reloading, it requires that a buffer knows when it has been modified
        std::unordered_set<unsigned int> CompiledShadersCache; // cache of already compiled shaders -> for reloading, it requires that a shader knows when it has been modified

        // Buffers
        std::shared_ptr<FixedBuffer<CameraBufferData, 1>> CameraBuffer;
        std::shared_ptr<FixedBuffer<TransformBufferData, 1>> TransformBuffer;
        std::shared_ptr<FixedBuffer<LightingBufferData, 1>> LightingBuffer;
        std::shared_ptr<DynamicBuffer<PointLightBufferData>> PointLightBuffer;
        std::shared_ptr<DynamicBuffer<DirLightBufferData>> DirLightBuffer;

        // UI
        UIPage* CurrentUIPage;

        // INTERNAL FUNCTIONALITIES ---------------------------------------------------------

        RenderContext& GetDefaultRenderContext();
        void BeforeDraw(RenderContext& renderContext);
        void AfterDraw(RenderContext& renderContext);
        bool IsMeshCached(unsigned int meshID);
        void CacheMesh(unsigned int meshID);
        void UncacheMesh(unsigned int meshID);
        bool IsShaderCached(unsigned int shaderID);
        void CacheShader(unsigned int shaderID);
        void UncacheShader(unsigned int shaderID);
        bool IsTextureCached(unsigned int textureID);
        void CacheTexture(unsigned int textureID);
        void UncacheTexture(unsigned int textureID);
        bool IsBufferCached(unsigned int bufferID);
        void CacheBuffer(unsigned int bufferID);
        void UncacheBuffer(unsigned int bufferID);
        void LoadModels(Scene& scene, std::unordered_set<unsigned int>& outLoadedMeshesIDs);
        void LoadMeshAndCache(Mesh& mesh);
        unsigned int LoadMesh(Mesh& mesh);
        void FreeUnusedMeshes(const std::unordered_set<unsigned int>& usedMeshesIDs);
        void FreeMeshes(const std::unordered_set<unsigned int>& meshesToFree);
        void FreeMesh(unsigned int meshID);
        void LoadMaterialsData(Scene& scene, std::unordered_set<unsigned int>& outLoadedTexturesIDs, std::unordered_set<unsigned int>& outLoadedBuffersIDs);
        void LoadMaterialData(std::shared_ptr<Material> material, std::unordered_set<unsigned int>& outLoadedTextures, std::unordered_set<unsigned int>& outLoadedBuffers);
        void LoadTextureAndCache(std::shared_ptr<Texture> texture);
        unsigned int LoadTexture(std::shared_ptr<Texture> texture);
        void LoadBufferAndCache(IBuffer* buffer);
        unsigned int LoadBuffer(IBuffer* buffer);
        void FreeUnusedTextures(const std::unordered_set<unsigned int>& usedTexturesIDs);
        void FreeTextures(const std::unordered_set<unsigned int>& texturesToFree);
        void FreeTexture(unsigned int textureID);
        void FreeUnusedBuffers(const std::unordered_set<unsigned int>& usedBuffersIDs);
        void FreeBuffers(const std::unordered_set<unsigned int>& buffersToFree);
        void FreeBuffer(unsigned int bufferID);
        void InitCameraDataBuffer();
        void InitTransformDataBuffer();
        void LoadCameraData(const Camera& camera);
        void LoadTransformData(const Transform& transform);
        void InitLightingDataBuffer();
        void LoadLightingData(const Scene& scene);
        void InitPointLightDataBuffer();
        void LoadPointLightData(const std::vector<PointLight>& pointLights);
        void InitDirLightDataBuffer();
        void LoadDirLightData(const std::vector<DirectionalLight>& dirLights);
        void SetRenderBufferTarget(std::shared_ptr<RenderBuffer> renderBuffer);
        void UnsetRenderBufferTarget(std::shared_ptr<RenderBuffer> renderBuffer);
        void SwapMainWindowBuffers();
        void BeforeDrawUI();
	};
}
