
#include "Renderer.h"

#include <memory>
#include <unordered_set>

#include "GPUResourceInspector.h"

#include <Renderer/Entities/Model.h>
#include <Renderer/Entities/Mesh.h>
#include <Renderer/Entities/Camera.h>
#include <Renderer/Entities/Scene.h>
#include <Renderer/Entities/SceneObject.h>
#include <Renderer/Entities/ShaderPipeline.h>
#include <Renderer/Entities/Texture.h>

#include "LayerAPI/OpenGL/RendererGL.h"
#include "Entities/RenderBuffer.h"
#include "LayerAPI/IRendererAPI.h"
#include "CommandBuffer.h"

#include <Renderer/Entities/BufferData/CameraBufferData.h>
#include <Renderer/Entities/BufferData/TransformBufferData.h>

#include <Utils/Log.h>
#include <Utils/FileLoader.h>

#include <unordered_set>

#define GH_DEFAULT_RENDER_BUFFER_SIZE glm::uvec2{ 1920, 1080 }
#define GH_CAMERA_DATA_BUFFER_NAME "CameraData"
#define GH_TRANSFORM_DATA_BUFFER_NAME "TransformData"

namespace GaladHen
{
    namespace Renderer
    {
        namespace // renderer's private namespace
        {
            // RENDERER DATA -----------------------------------------------------------------------------------------------------------------------------------------

            // Static Renderer Data
            API CurrentAPI = API::OpenGL;
            std::unique_ptr<IRendererAPI> RendererAPI = nullptr; // API-specific renderer for API-specific operations
            bool Initialized = false;

            enum class RenderContextType
            {
                SingleBuffering,
                DoubleBuffering
            };

            // A render context is a set of data for a series of related draw calls (e.g. same render buffer, same camera, ...)
            class RenderContext
            {
            public:
                // RenderScene -> collection of models and theirs Bounding Volumes + spatial partitioning structure containing all the models (for frustum culling)

                RenderContext(unsigned int width, unsigned int height, RenderContextType renderContextType)
                    : FrontBuffer(CreateRenderBuffer(width, height))
                    , BackBuffer(std::shared_ptr<RenderBuffer>{})
                    , RenderContextType(renderContextType)
                {
                    if (RenderContextType == RenderContextType::DoubleBuffering)
                    {
                        BackBuffer = CreateRenderBuffer(width, height);
                    }
                    else
                    {
                        BackBuffer = FrontBuffer;
                    }
                }

                RenderContextType GetRenderContextType()
                {
                    return RenderContextType;
                }

                std::shared_ptr<RenderBuffer> GetFrontBuffer()
                {
                    return FrontBuffer;
                }

                std::shared_ptr<RenderBuffer> GetBackBuffer()
                {
                    return BackBuffer;
                }

                void SwapBuffers()
                {
                    if (RenderContextType == RenderContextType::DoubleBuffering)
                        std::swap(FrontBuffer, BackBuffer);
                }

                Camera RenderingCamera;

            private:

                RenderContextType RenderContextType;
                // RenderBuffer(s) -> two in case of multiple buffering
                std::shared_ptr<RenderBuffer> FrontBuffer;
                std::shared_ptr<RenderBuffer> BackBuffer;

            };

            // Rendering Data
            IdList<RenderContext> RenderContexts; // the RenderContext with id 1 is the default one
            std::unordered_set<unsigned int> LoadedMeshesCache; // cache of already loaded meshes -> for reloading, it requires that a mesh knows when it has been modified
            std::unordered_set<unsigned int> LoadedTexturesCache; // cache of already loaded textures -> for reloading, it requires that a texture knows when it has been modified
            std::unordered_set<unsigned int> LoadedBuffersCache; // cache of already loaded buffers -> for reloading, it requires that a buffer knows when it has been modified
            std::unordered_set<unsigned int> CompiledShadersCache; // cache of already compiled shaders -> for reloading, it requires that a shader knows when it has been modified
            // Buffers
            std::shared_ptr<FixedBuffer<CameraBufferData, 1>> CameraBuffer;
            std::shared_ptr<FixedBuffer<TransformBufferData, 1>> TransformBuffer;
            //std::shared_ptr<Buffer> LightingDataBuffer;

            // RENDERER FUNCTIONALITIES ------------------------------------------------------------------------------------------------------------------------------

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
            void SetRenderBufferTarget(std::shared_ptr<RenderBuffer> renderBuffer);
            void UnsetRenderBufferTarget(std::shared_ptr<RenderBuffer> renderBuffer);

            RenderContext& GetDefaultRenderContext()
            {
                return RenderContexts.GetObjectWithId(1);
            }

            void BeforeDraw(RenderContext& renderContext)
            {
                // Operations needed before drawing

                // Clear back render buffer
                ClearRenderBuffer(renderContext.GetBackBuffer());

                // Set render context's back buffer as target for next draw calls
                SetRenderBufferTarget(renderContext.GetBackBuffer());
            }

            void AfterDraw(RenderContext& renderContext)
            {
                // Operations needed after drawing

                // Unbind render context's back buffer as target
                UnsetRenderBufferTarget(renderContext.GetBackBuffer());

                // Swap buffers
                renderContext.SwapBuffers();
            }

            bool IsMeshCached(unsigned int meshID)
            {
                if (LoadedMeshesCache.find(meshID) != LoadedMeshesCache.end())
                {
                    return true;
                }

                return false;
            }

            void CacheMesh(unsigned int meshID)
            {
                LoadedMeshesCache.insert(meshID);
            }

            void UncacheMesh(unsigned int meshID)
            {
                LoadedMeshesCache.erase(meshID);
            }

            bool IsShaderCached(unsigned int shaderID)
            {
                if (CompiledShadersCache.find(shaderID) != CompiledShadersCache.end())
                {
                    return true;
                }

                return false;
            }

            void CacheShader(unsigned int shaderID)
            {
                CompiledShadersCache.insert(shaderID);
            }

            void UncacheShader(unsigned int shaderID)
            {
                CompiledShadersCache.erase(shaderID);
            }

            bool IsTextureCached(unsigned int textureID)
            {
                if (LoadedTexturesCache.find(textureID) != LoadedTexturesCache.end())
                {
                    return true;
                }

                return false;
            }

            void CacheTexture(unsigned int textureID)
            {
                LoadedTexturesCache.insert(textureID);
            }

            void UncacheTexture(unsigned int textureID)
            {
                LoadedTexturesCache.erase(textureID);
            }

            bool IsBufferCached(unsigned int bufferID)
            {
                if (LoadedBuffersCache.find(bufferID) != LoadedBuffersCache.end())
                {
                    return true;
                }

                return false;
            }

            void CacheBuffer(unsigned int bufferID)
            {
                LoadedBuffersCache.insert(bufferID);
            }

            void UncacheBuffer(unsigned int bufferID)
            {
                LoadedBuffersCache.erase(bufferID);
            }

            void LoadModels(Scene& scene, std::unordered_set<unsigned int>& outLoadedMeshesIDs)
            {
                for (SceneObject& sceneObject : scene.SceneObjects)
                {
                    std::shared_ptr<Model> model = std::shared_ptr<Model>(sceneObject.GetSceneObjectModel());

                    for (Mesh& mesh : model->GetMeshes())
                    {
                        LoadMeshAndCache(mesh);

                        unsigned int meshID = GPUResourceInspector::GetResourceID(&mesh);
                        outLoadedMeshesIDs.insert(meshID);
                    }
                }
            }

            void LoadMeshAndCache(Mesh& mesh)
            {
                unsigned int meshID = GPUResourceInspector::GetResourceID(&mesh);

                if (IsMeshCached(meshID) && mesh.IsResourceValid())
                {
                    return;
                }
                
                unsigned int newId = LoadMesh(mesh);
                CacheMesh(newId);
            }

            unsigned int LoadMesh(Mesh& mesh)
            {
                CommandBuffer<MemoryTransferCommand> memoryCommands;
                memoryCommands.emplace_back(MemoryTransferCommand{});

                MemoryTransferCommand& command = memoryCommands[0];
                command.Data = &mesh;
                command.MemoryTargetID = 0;
                command.TargetType = MemoryTargetType::Mesh;
                command.TransferType = MemoryTransferType::Load;

                // Load and assign id

                RendererAPI->TransferData(memoryCommands);
                GPUResourceInspector::SetResourceID(&mesh, command.MemoryTargetID);

                return command.MemoryTargetID;
            }

            void FreeUnusedMeshes(const std::unordered_set<unsigned int>& usedMeshesIDs)
            {
                std::unordered_set<unsigned int> meshesToFree;
                for (unsigned int id : LoadedMeshesCache)
                {
                    if (usedMeshesIDs.find(id) == usedMeshesIDs.end())
                    {
                        meshesToFree.insert(id);
                    }
                }

                FreeMeshes(meshesToFree);
            }

            void FreeMeshes(const std::unordered_set<unsigned int>& meshesToFree)
            {
                for (unsigned int meshID : meshesToFree)
                {
                    FreeMesh(meshID);
                }
            }

            void FreeMesh(unsigned int meshID)
            {
                if (!IsMeshCached(meshID))
                    return;

                CommandBuffer<MemoryTransferCommand> memoryCommands;
                memoryCommands.emplace_back(MemoryTransferCommand{});

                MemoryTransferCommand& command = memoryCommands[0];
                command.MemoryTargetID = meshID;
                command.TargetType = MemoryTargetType::Mesh;
                command.TransferType = MemoryTransferType::Free;

                // Free and remove from cache
                RendererAPI->TransferData(memoryCommands);
                LoadedMeshesCache.erase(meshID);
            }

            void LoadMaterialsData(Scene& scene, std::unordered_set<unsigned int>& outLoadedTexturesIDs, std::unordered_set<unsigned int>& outLoadedBuffersIDs)
            {
                for (SceneObject& sceneObject : scene.SceneObjects)
                {
                    std::vector<std::shared_ptr<Material>> materials = sceneObject.GetSceneObjectMaterials();

                    for (std::shared_ptr<Material>& material : materials)
                    {
                        LoadMaterialData(material, outLoadedTexturesIDs, outLoadedBuffersIDs);
                    }
                }
            }

            void LoadMaterialData(std::shared_ptr<Material> material, std::unordered_set<unsigned int>& outLoadedTextures, std::unordered_set<unsigned int>& outLoadedBuffers)
            {
                CommandBuffer<MemoryTransferCommand> commandBuffer; // TODO: send all with one request?

                // Load textures
                for (std::pair<const std::string, std::shared_ptr<Texture>>& textureData : material->TextureData)
                {
                    LoadTextureAndCache(textureData.second);
                    outLoadedTextures.insert(GPUResourceInspector::GetResourceID(textureData.second.get()));
                }

                // Load buffers
                for (std::pair<const std::string, std::shared_ptr<IBuffer>>& bufferData : material->BufferData)
                {
                    LoadBufferAndCache(bufferData.second.get());
                    outLoadedBuffers.insert(GPUResourceInspector::GetResourceID(bufferData.second.get()));
                }
            }

            void LoadTextureAndCache(std::shared_ptr<Texture> texture)
            {
                unsigned int textureID = GPUResourceInspector::GetResourceID(texture.get());

                if (IsTextureCached(textureID) && texture->IsResourceValid())
                {
                    return;
                }

                unsigned int newId = LoadTexture(texture);
                CacheTexture(newId);
            }

            unsigned int LoadTexture(std::shared_ptr<Texture> texture)
            {
                CommandBuffer<MemoryTransferCommand> commandBuffer;
                commandBuffer.emplace_back(MemoryTransferCommand{});

                MemoryTransferCommand& command = commandBuffer[0];
                command.Data = texture.get();
                command.MemoryTargetID = 0;
                command.TargetType = MemoryTargetType::Texture;
                command.TransferType = MemoryTransferType::Load;

                // Load, assign id and cache

                RendererAPI->TransferData(commandBuffer);
                GPUResourceInspector::SetResourceID(texture.get(), command.MemoryTargetID);

                return command.MemoryTargetID;
            }

            void LoadBufferAndCache(IBuffer* buffer)
            {
                unsigned int bufferID = GPUResourceInspector::GetResourceID(buffer);

                if (IsBufferCached(bufferID) && buffer->IsResourceValid())
                {
                    return;
                }

                unsigned int newId = LoadBuffer(buffer);
                CacheBuffer(newId);
            }

            unsigned int LoadBuffer(IBuffer* buffer)
            {
                CommandBuffer<MemoryTransferCommand> commandBuffer;
                commandBuffer.emplace_back(MemoryTransferCommand{});

                MemoryTransferCommand& command = commandBuffer[0];
                command.Data = buffer;
                command.MemoryTargetID = 0;
                command.TargetType = MemoryTargetType::Buffer;
                command.TransferType = MemoryTransferType::Load;

                // Load and assign id

                RendererAPI->TransferData(commandBuffer);
                GPUResourceInspector::SetResourceID(buffer, command.MemoryTargetID);

                return command.MemoryTargetID;
            }

            void FreeUnusedTextures(const std::unordered_set<unsigned int>& usedTexturesIDs)
            {
                std::unordered_set<unsigned int> texturesToFree;
                for (unsigned int id : LoadedTexturesCache)
                {
                    if (usedTexturesIDs.find(id) == usedTexturesIDs.end())
                    {
                        texturesToFree.insert(id);
                    }
                }
                FreeTextures(texturesToFree);
            }

            void FreeTextures(const std::unordered_set<unsigned int>& texturesToFree)
            {
                for (unsigned int textureID : texturesToFree)
                {
                    FreeTexture(textureID);
                }
            }

            void FreeTexture(unsigned int textureID)
            {
                if (!IsTextureCached(textureID))
                    return;

                CommandBuffer<MemoryTransferCommand> memoryCommands;
                memoryCommands.emplace_back(MemoryTransferCommand{});

                MemoryTransferCommand& command = memoryCommands[0];
                command.MemoryTargetID = textureID;
                command.TargetType = MemoryTargetType::Texture;
                command.TransferType = MemoryTransferType::Free;

                // Free and remove from cache
                RendererAPI->TransferData(memoryCommands);
                LoadedTexturesCache.erase(textureID);
            }

            void FreeUnusedBuffers(const std::unordered_set<unsigned int>& usedBuffersIDs)
            {
                std::unordered_set<unsigned int> buffersToFree;
                for (unsigned int id : LoadedBuffersCache)
                {
                    if (usedBuffersIDs.find(id) == usedBuffersIDs.end())
                    {
                        buffersToFree.insert(id);
                    }
                }
                FreeBuffers(buffersToFree);
            }

            void FreeBuffers(const std::unordered_set<unsigned int>& buffersToFree)
            {
                for (unsigned int bufferID : buffersToFree)
                {
                    FreeBuffer(bufferID);
                }
            }

            void FreeBuffer(unsigned int bufferID)
            {
                if (!IsBufferCached(bufferID))
                    return;

                CommandBuffer<MemoryTransferCommand> memoryCommands;
                memoryCommands.emplace_back(MemoryTransferCommand{});

                MemoryTransferCommand& command = memoryCommands[0];
                command.MemoryTargetID = bufferID;
                command.TargetType = MemoryTargetType::Buffer;
                command.TransferType = MemoryTransferType::Free;

                // Free and remove from cache
                RendererAPI->TransferData(memoryCommands);
                LoadedBuffersCache.erase(bufferID);
            }

            void InitCameraDataBuffer()
            {
                FixedBuffer<CameraBufferData, 1>* data = new FixedBuffer<CameraBufferData, 1>();

                CameraBuffer = std::shared_ptr<FixedBuffer<CameraBufferData, 1>>(data);
                
                // TODO: populate buffer basing on API (to match shader data structure)
            }

            void InitTransformDataBuffer()
            {
                FixedBuffer<TransformBufferData, 1>* data = new FixedBuffer<TransformBufferData, 1>();

                TransformBuffer = std::shared_ptr<FixedBuffer<TransformBufferData, 1>>(data);

                // TODO: populate buffer basing on API (to match shader data structure)
            }

            void LoadCameraData(const Camera& camera)
            {
                CameraBufferData data{};
                data.ViewMatrix = camera.GetViewMatrix();
                data.ProjectionMatrix = camera.GetProjectionMatrix();
                data.CameraPosition = camera.Transform.GetPosition();
                CameraBuffer->SetData(data, 0);

                LoadBuffer(CameraBuffer.get());
            }

            void LoadTransformData(const Transform& transform)
            {
                TransformBufferData data{};
                data.ModelMatrix = transform.ToMatrix();
                data.NormalMatrix = glm::inverse(glm::transpose(data.ModelMatrix));
                TransformBuffer->SetData(data, 0);

                LoadBuffer(TransformBuffer.get());
            }

            void SetRenderBufferTarget(std::shared_ptr<RenderBuffer> renderBuffer)
            {
                RendererAPI->BindRenderBuffer(GPUResourceInspector::GetResourceID(renderBuffer.get()));
            }

            void UnsetRenderBufferTarget(std::shared_ptr<RenderBuffer> renderBuffer)
            {
                RendererAPI->UnbindActiveRenderBuffer();
            }
        }

        void Init(API apiToUse)
        {
            // For now the initialization is skipped if already done -> TODO: reinitialize all the renderer at api change
            if (Initialized)
            {
                Log::Warning("Renderer.cpp", "Call to Init(), but Renderer is already initialized.");

                return;
            }

            // Initialization basing on API

            switch (apiToUse)
            {
            default:
            case API::OpenGL:
            {
                // Create low level OpenGL Renderer and init static data
                CurrentAPI = API::OpenGL;
                RendererAPI = std::unique_ptr<RendererGL>{ new RendererGL{} };

                break;
            }
            }

            RendererAPI->Init();

            // Create default RenderContext
            RenderContexts.AddWithId(RenderContext{ GH_DEFAULT_RENDER_BUFFER_SIZE.x, GH_DEFAULT_RENDER_BUFFER_SIZE.y, RenderContextType::DoubleBuffering });

            RendererAPI->EnableDepthTest(true);

            // Create default buffers
            InitCameraDataBuffer();
            LoadCameraData(Camera{});
            InitTransformDataBuffer();
            LoadTransformData(Transform{});

            Initialized = true;
        }

        std::shared_ptr<RenderBuffer> CreateRenderBuffer(unsigned int width, unsigned int height)
        {
            // Create object
            std::shared_ptr<RenderBuffer> renderBuffer = std::shared_ptr<RenderBuffer>{ new RenderBuffer{ width, height } };

            // Create render buffer at api level and assing id
            unsigned int id = RendererAPI->CreateRenderBuffer(width, height);
            GPUResourceInspector::SetResourceID(renderBuffer.get(), id);

            return renderBuffer;
        }

        std::shared_ptr<RenderBuffer> GetFrontRenderBuffer()
        {
            return GetDefaultRenderContext().GetFrontBuffer();
        }

        GLuint GetRenderBufferColorApiID(std::shared_ptr<RenderBuffer> renderBuffer)
        {
            return RendererAPI->GetRenderBufferColorApiID(GPUResourceInspector::GetResourceID(renderBuffer.get()));
        }

        void ClearRenderBuffer(std::shared_ptr<RenderBuffer> renderBuffer)
        {
            RendererAPI->ClearRenderBuffer(GPUResourceInspector::GetResourceID(renderBuffer.get()), renderBuffer->ClearColor);
        }

        void Draw(Scene& scene)
        {
            BeforeDraw(GetDefaultRenderContext());

            std::unordered_set<unsigned int> loadedMeshesIDs;
            LoadModels(scene, loadedMeshesIDs);
            CompileShaders(scene);
            std::unordered_set<unsigned int> loadedTexturesIDs, loadedBuffersIDs;
            LoadMaterialsData(scene, loadedTexturesIDs, loadedBuffersIDs);
            LoadCameraData(scene.MainCamera);

            // TODO: load lights

            // TODO: instanced draw

            for (SceneObject& sceneObject : scene.SceneObjects)
            {
                CommandBuffer<RenderCommand> renderBuffer;

                std::shared_ptr<Model> model = std::shared_ptr<Model>(sceneObject.GetSceneObjectModel());

                unsigned int index = 0;
                for (Mesh& mesh : model->GetMeshes())
                {
                    RenderCommand command;
                    command.DataSourceID = GPUResourceInspector::GetResourceID(&mesh);
                    command.Material = sceneObject.GetMaterial(index);
                    command.ShaderSourceID = GPUResourceInspector::GetResourceID(command.Material->GetPipeline().get());

                    // Add common rendering data
                    command.AdditionalBufferData.emplace(GH_CAMERA_DATA_BUFFER_NAME, CameraBuffer);
                    LoadTransformData(sceneObject.Transform);
                    command.AdditionalBufferData.emplace(GH_TRANSFORM_DATA_BUFFER_NAME, TransformBuffer);

                    renderBuffer.emplace_back(command);
                }

                RendererAPI->Draw(renderBuffer);
                // A single Render Command at time because at the moment we use a TransformDataBuffer that invalidates itself when changing data ->
                // passing the same TransformDataBuffer inside the AdditionalBufferData means using the same buffer but also the latest same data stored inside it
                // (we should have instead a different buffer for each Render Command)
            }

            // Free unused gpu data
            FreeUnusedMeshes(loadedMeshesIDs);
            FreeUnusedTextures(loadedTexturesIDs);
            FreeUnusedBuffers(loadedBuffersIDs);

            AfterDraw(GetDefaultRenderContext());
        }

        //void UpdateModel(const Model& model)
        //{
        //    for (const Mesh& mesh : model.GetMeshes())
        //    {
        //        UpdateMesh(mesh);
        //    }
        //}

        //void UpdateMesh(const Mesh& mesh)
        //{
        //    unsigned int meshID = GPUResourceInspector::GetResourceID(&mesh);

        //    if (IsMeshCached(meshID))
        //        return;

        //    CommandBuffer<MemoryTransferCommand> memoryBuffer;
        //    memoryBuffer.push_back(MemoryTransferCommand{});
        //    MemoryTransferCommand& command = memoryBuffer[0];

        //    command.Data = (void*)&mesh;
        //    command.MemoryTargetID = 0;
        //    command.TargetType = MemoryTargetType::Mesh;
        //    command.TransferType = MemoryTransferType::Load;

        //    RendererAPI->TransferData(memoryBuffer);

        //    // No need to retrieve mesh id, already in cache
        //}

        bool CompileShaders(const Scene& scene)
        {
            bool success = true;

            for (const SceneObject& sceneObj : scene.SceneObjects)
            {
                std::vector<std::shared_ptr<Material>> materials = sceneObj.GetSceneObjectMaterials();
                
                for (std::shared_ptr<Material>& material : materials)
                {
                    std::shared_ptr<ShaderPipeline> shader = material->GetPipeline();
                    unsigned int shaderID = GPUResourceInspector::GetResourceID(shader.get());

                    // Compile, set id and cache
                    success &= CompileShader(*shader);
                }
            }

            // Here we could delete unused shaders from cache, but for now it is not needed

            return success;
        }

        bool CompileShader(ShaderPipeline& program)
        {
            unsigned int shaderID = GPUResourceInspector::GetResourceID(&program);

            if (IsShaderCached(shaderID) && program.IsResourceValid())
                return true;

            CommandBuffer<CompileCommand> compileBuffer;
            compileBuffer.emplace_back(CompileCommand{});
            CompileCommand& command = compileBuffer[0];

            if (program.GetType() == ShaderPipelineType::ShaderPipeline)
            {
                command.VertexCode = FileLoader::ReadTextFile(program.GetVertexShaderPath().data());
                command.TessContCode = FileLoader::ReadTextFile(program.GetTessContShaderPath().data());
                command.TessEvalCode = FileLoader::ReadTextFile(program.GetTessEvalShaderPath().data());
                command.GeometryCode = FileLoader::ReadTextFile(program.GetGeometryShaderPath().data());
                command.FragmentCode = FileLoader::ReadTextFile(program.GetFragmentShaderPath().data());
            }
            else
            {
                command.ComputeCode = FileLoader::ReadTextFile(program.GetComputeShaderPath().data());
            }

            command.ShaderPipelineID = 0;
            command.Type = CompileType::Compile;

            RendererAPI->Compile(compileBuffer);

            if (command.Result.Succeed)
            {
                if (command.Result.Description.length() > 0)
                {
                    // Non fatal errors
                    Log::Warning("Renderer", command.Result.Description);
                }

                // Assign id and cache
                GPUResourceInspector::SetResourceID(&program, command.ShaderPipelineID);
                CacheShader(command.ShaderPipelineID);

                return true;
            }

            // Fatal errors
            Log::Error("Renderer", command.Result.Description);

            return false;
        }
    }
}
