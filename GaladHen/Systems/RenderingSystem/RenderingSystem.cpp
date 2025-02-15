
#include "RenderingSystem.h"

#include <Utils/log.h>
#include "LayerAPI/OpenGL/RendererGL.h"
#include "GPUResourceInspector.h"
#include "Entities/SceneObject.h"
#include "CommandBuffer.h"
#include "ShaderPreprocessor/ShaderPreprocessor.h"
#include <Utils/FileLoader.h>
#include "Entities/RenderBuffer.h"
#include "Entities/Scene.h"
#include "Entities/Model.h"
#include "Entities/ShaderPipeline.h"
#include "Entities/Texture.h"

#define GH_DEFAULT_RENDER_BUFFER_WIDTH 1920
#define GH_DEFAULT_RENDER_BUFFER_HEIGHT 1080
#define GH_CAMERA_DATA_BUFFER_NAME "CameraData"
#define GH_TRANSFORM_DATA_BUFFER_NAME "TransformData"
#define GH_LIGHTING_DATA_BUFFER_NAME "LightingData"
#define GH_POINTLIGHT_DATA_BUFFER_NAME "PointLightBuffer"
#define GH_DIRLIGHT_DATA_BUFFER_NAME "DirectionalLightBuffer"

namespace GaladHen
{
    RenderingSystem::RenderingSystem()
        : CurrentAPI(GH_CURRENT_API)
        , RendererAPI(nullptr)
        , Initialized(false)
        , CurrentUIPage(nullptr)
        , CameraBuffer(FixedBuffer<CameraBufferData, 1>{ BufferType::Uniform, BufferAccessType::StaticRead }) // TODO: populate buffer basing on API (to match shader data structure)
        , TransformBuffer(FixedBuffer<TransformBufferData, 1>{ BufferType::Uniform, BufferAccessType::StaticRead }) // TODO: populate buffer basing on API (to match shader data structure)
        , LightingBuffer(FixedBuffer<LightingBufferData, 1>{ BufferType::Uniform, BufferAccessType::StaticRead }) // TODO: populate buffer basing on API (to match shader data structure)s
        , PointLightBuffer(DynamicBuffer<PointLightBufferData>{ BufferType::ShaderStorage, BufferAccessType::StaticRead }) // TODO: populate buffer basing on API (to match shader data structure)
        , DirLightBuffer(DynamicBuffer<DirLightBufferData>{ BufferType::ShaderStorage, BufferAccessType::StaticRead }) // TODO: populate buffer basing on API (to match shader data structure)
    {}

    std::weak_ptr<RenderBuffer> RenderingSystem::CreateRenderBuffer(unsigned int width, unsigned int height)
    {
        // Create object
        std::shared_ptr<RenderBuffer> renderBuffer = std::shared_ptr<RenderBuffer>{ new RenderBuffer{ width, height } };
        RenderBuffers.emplace_back(renderBuffer); // rendering system ownership

        // Create render buffer at api level and assing id
        unsigned int id = RendererAPI->CreateRenderBuffer(width, height);
        GPUResourceInspector::SetResourceID(renderBuffer.get(), id);
        GPUResourceInspector::ValidateResource(renderBuffer.get());

        return renderBuffer;
    }

    std::weak_ptr<RenderBuffer> RenderingSystem::GetFrontRenderBuffer()
    {
        return GetDefaultRenderContext().GetFrontBuffer();
    }

    unsigned int RenderingSystem::GetRenderBufferColorApiID(const RenderBuffer& renderBuffer)
    {
        return RendererAPI->GetRenderBufferColorApiID(GPUResourceInspector::GetResourceID(&renderBuffer));
    }

    void RenderingSystem::ClearRenderBuffer(const RenderBuffer& renderBuffer)
    {
        RendererAPI->ClearRenderBuffer(GPUResourceInspector::GetResourceID(&renderBuffer), renderBuffer.ClearColor);
    }

    void RenderingSystem::Draw(const Scene& scene)
    {
        BeforeDraw(GetDefaultRenderContext());

        std::unordered_set<unsigned int> loadedMeshesIDs;
        LoadModels(scene, loadedMeshesIDs);
        CompileShaders(scene);
        std::unordered_set<unsigned int> loadedTexturesIDs, loadedBuffersIDs;
        LoadMaterialsData(scene, loadedTexturesIDs, loadedBuffersIDs);
        LoadCameraData(scene.MainCamera);

        LoadLightingData(scene);
        LoadPointLightData(scene.PointLights);
        LoadDirLightData(scene.DirectionalLights);

        // TODO: instanced draw

        for (const SceneObject& sceneObject : scene.SceneObjects)
        {
            CommandBuffer<RenderCommand> renderBuffer;

            std::weak_ptr<Model> model = sceneObject.GetSceneObjectModel();
            std::shared_ptr<Model> shModel = model.lock();
            if (!shModel)
                continue;

            unsigned int index = 0;
            for (Mesh& mesh : shModel->Meshes)
            {
                RenderCommand command;
                command.DataSourceID = GPUResourceInspector::GetResourceID(&mesh);
                command.Material = sceneObject.GetMaterial(index).lock().get();
                command.ShaderSourceID = GPUResourceInspector::GetResourceID(command.Material->GetPipeline().lock().get());

                // Add common rendering data
                command.AdditionalBufferData.emplace(GH_CAMERA_DATA_BUFFER_NAME, &CameraBuffer);
                LoadTransformData(sceneObject.Transform);
                command.AdditionalBufferData.emplace(GH_TRANSFORM_DATA_BUFFER_NAME, &TransformBuffer);
                command.AdditionalBufferData.emplace(GH_LIGHTING_DATA_BUFFER_NAME, &LightingBuffer);
                command.AdditionalBufferData.emplace(GH_POINTLIGHT_DATA_BUFFER_NAME, &PointLightBuffer);
                command.AdditionalBufferData.emplace(GH_DIRLIGHT_DATA_BUFFER_NAME, &DirLightBuffer);

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

    void RenderingSystem::DrawUI()
    {
        // First call new frame functionalities for UI
        BeforeDrawUI();

        // Build UIPage
        CurrentUIPage->Build();

        // UI draw call
        RendererAPI->DrawUI();
    }

    bool RenderingSystem::CompileShaders(const Scene& scene)
    {
        bool success = true;

        for (const SceneObject& sceneObj : scene.SceneObjects)
        {
            std::vector<std::weak_ptr<Material>> materials = sceneObj.GetSceneObjectMaterials();

            for (std::weak_ptr<Material>& material : materials)
            {
                if (std::shared_ptr<Material> shMaterial = material.lock())
                {
                    std::weak_ptr<ShaderPipeline> shader = shMaterial->GetPipeline();
                    ShaderPipeline* pipeline = shader.lock().get();
                    unsigned int shaderID = GPUResourceInspector::GetResourceID(pipeline);

                    if (pipeline)
                    {
                        // Compile, set id and cache
                        success &= CompileShader(*pipeline);
                    }
                }
            }
        }

        // Here we could delete unused shaders from cache, but for now it is not needed

        return success;
    }

    bool RenderingSystem::CompileShader(ShaderPipeline& program)
    {
        unsigned int shaderID = GPUResourceInspector::GetResourceID(&program);

        if (IsShaderCached(shaderID) && program.IsResourceValid())
            return true;

        CommandBuffer<CompileCommand> compileBuffer;
        compileBuffer.emplace_back(CompileCommand{});
        CompileCommand& command = compileBuffer[0];

        if (program.GetType() == ShaderPipelineType::ShaderPipeline)
        {
            command.VertexCode = ShaderPreprocessor::PreprocessShader(program.GetVertexShaderPath().data(), CurrentAPI);
            command.TessContCode = ShaderPreprocessor::PreprocessShader(program.GetTessContShaderPath().data(), CurrentAPI);
            command.TessEvalCode = ShaderPreprocessor::PreprocessShader(program.GetTessEvalShaderPath().data(), CurrentAPI);
            command.GeometryCode = ShaderPreprocessor::PreprocessShader(program.GetGeometryShaderPath().data(), CurrentAPI);
            command.FragmentCode = ShaderPreprocessor::PreprocessShader(program.GetFragmentShaderPath().data(), CurrentAPI);
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
            GPUResourceInspector::ValidateResource(&program);
            CacheShader(command.ShaderPipelineID);

            return true;
        }

        // Fatal errors
        Log::Error("Renderer", command.Result.Description);

        return false;
    }

    void RenderingSystem::CreateMainWindow(const std::string& name, const glm::uvec2 size)
    {
        RendererAPI->CreateRenderingWindow(name.data(), size);
    }

    void RenderingSystem::CloseMainWindow()
    {
        RendererAPI->CloseRenderingWindow();
    }

    void RenderingSystem::Init()
    {
        // For now the initialization is skipped if already done -> TODO: reinitialize all the renderer at api change
        if (Initialized)
        {
            Log::Warning("RenderingSystem.cpp", "Call to Init(), but Renderer is already initialized.");

            return;
        }

        // Initialization basing on API

        switch (CurrentAPI)
        {
        default:
        case API::OpenGL:
        {
            // Create low level OpenGL Renderer and init static data
            //CurrentAPI = API::OpenGL;
            RendererAPI = new RendererGL{};

            break;
        }
        }

        RendererAPI->Init();
        RendererAPI->InitUI();

        // Create default RenderContext
        RenderContexts.AddWithId(RenderContext{ *this, GH_DEFAULT_RENDER_BUFFER_WIDTH, GH_DEFAULT_RENDER_BUFFER_HEIGHT, RenderContextType::DoubleBuffering });

        RendererAPI->EnableDepthTest(true);
        RendererAPI->EnableBackFaceCulling(true);

        // Load default buffers
        LoadCameraData(Camera{});
        LoadTransformData(Transform{});

        Initialized = true;
    }

    void RenderingSystem::Quit()
    {
        // TODO: Free resources
        if (CurrentUIPage)
            delete CurrentUIPage;

        delete RendererAPI;
    }

    RenderingSystem::RenderContext::RenderContext(RenderingSystem& renderingSys, unsigned int width, unsigned int height, RenderingSystem::RenderContextType renderContextType)
        : FrontBuffer(renderingSys.CreateRenderBuffer(width, height))
        , BackBuffer(std::shared_ptr<RenderBuffer>{})
        , RenderContextType(renderContextType)
    {
        if (RenderContextType == RenderContextType::DoubleBuffering)
        {
            BackBuffer = renderingSys.CreateRenderBuffer(width, height).lock();
        }
        else
        {
            BackBuffer = FrontBuffer;
        }
    }

    RenderingSystem::RenderContextType RenderingSystem::RenderContext::GetRenderContextType()
    {
        return RenderContextType;
    }

    std::weak_ptr<RenderBuffer> RenderingSystem::RenderContext::GetFrontBuffer() const
    {
        return FrontBuffer;
    }

    std::weak_ptr<RenderBuffer> RenderingSystem::RenderContext::GetBackBuffer() const
    {
        return BackBuffer;
    }

    void RenderingSystem::RenderContext::SwapBuffers()
    {
        if (RenderContextType == RenderContextType::DoubleBuffering)
            std::swap(FrontBuffer, BackBuffer);
    }


    RenderingSystem::RenderContext& RenderingSystem::GetDefaultRenderContext()
    {
        return RenderContexts.GetObjectWithId(1);
    }

    void RenderingSystem::BeforeDraw(const RenderContext& renderContext)
    {
        // Operations needed before drawing

        std::weak_ptr<RenderBuffer> backBuffer = renderContext.GetBackBuffer();
        if (std::shared_ptr<RenderBuffer> shBackBuffer = backBuffer.lock())
        {
            // Clear back render buffer
            ClearRenderBuffer(*shBackBuffer);

            // Set render context's back buffer as target for next draw calls
            SetRenderBufferTarget(*shBackBuffer);

            // Set viewport basing on render buffer resolution
            RendererAPI->SetViewport(glm::uvec2(0, 0), shBackBuffer->GetSize());
        }
    }

    void RenderingSystem::AfterDraw(RenderContext& renderContext)
    {
        // Operations needed after drawing

        // Unbind render context's back buffer as target
        std::weak_ptr<RenderBuffer> backBuffer = renderContext.GetBackBuffer();
        if (std::shared_ptr<RenderBuffer> shBackBuffer = backBuffer.lock())
        {
            UnsetRenderBufferTarget(*shBackBuffer);

            // Swap buffers
            renderContext.SwapBuffers();

            SwapMainWindowBuffers();
        }
    }

    bool RenderingSystem::IsMeshCached(unsigned int meshID)
    {
        if (LoadedMeshesCache.find(meshID) != LoadedMeshesCache.end())
        {
            return true;
        }

        return false;
    }

    void RenderingSystem::CacheMesh(unsigned int meshID)
    {
        LoadedMeshesCache.insert(meshID);
    }

    void RenderingSystem::UncacheMesh(unsigned int meshID)
    {
        LoadedMeshesCache.erase(meshID);
    }

    bool RenderingSystem::IsShaderCached(unsigned int shaderID)
    {
        if (CompiledShadersCache.find(shaderID) != CompiledShadersCache.end())
        {
            return true;
        }

        return false;
    }

    void RenderingSystem::CacheShader(unsigned int shaderID)
    {
        CompiledShadersCache.insert(shaderID);
    }

    void RenderingSystem::UncacheShader(unsigned int shaderID)
    {
        CompiledShadersCache.erase(shaderID);
    }

    bool RenderingSystem::IsTextureCached(unsigned int textureID)
    {
        if (LoadedTexturesCache.find(textureID) != LoadedTexturesCache.end())
        {
            return true;
        }

        return false;
    }

    void RenderingSystem::CacheTexture(unsigned int textureID)
    {
        LoadedTexturesCache.insert(textureID);
    }

    void RenderingSystem::UncacheTexture(unsigned int textureID)
    {
        LoadedTexturesCache.erase(textureID);
    }

    bool RenderingSystem::IsBufferCached(unsigned int bufferID)
    {
        if (LoadedBuffersCache.find(bufferID) != LoadedBuffersCache.end())
        {
            return true;
        }

        return false;
    }

    void RenderingSystem::CacheBuffer(unsigned int bufferID)
    {
        LoadedBuffersCache.insert(bufferID);
    }

    void RenderingSystem::UncacheBuffer(unsigned int bufferID)
    {
        LoadedBuffersCache.erase(bufferID);
    }

    void RenderingSystem::LoadModels(const Scene& scene, std::unordered_set<unsigned int>& outLoadedMeshesIDs)
    {
        for (const SceneObject& sceneObject : scene.SceneObjects)
        {
            std::weak_ptr<Model> model = sceneObject.GetSceneObjectModel();
            if (std::shared_ptr<Model> shModel = model.lock())
            {
                for (Mesh& mesh : shModel->Meshes)
                {
                    LoadMeshAndCache(mesh);

                    unsigned int meshID = GPUResourceInspector::GetResourceID(&mesh);
                    outLoadedMeshesIDs.insert(meshID);
                }
            }
        }
    }

    void RenderingSystem::LoadMeshAndCache(Mesh& mesh)
    {
        unsigned int meshID = GPUResourceInspector::GetResourceID(&mesh);

        if (IsMeshCached(meshID) && mesh.IsResourceValid())
        {
            return;
        }

        unsigned int newId = LoadMesh(mesh);
        CacheMesh(newId);
    }

    unsigned int RenderingSystem::LoadMesh(Mesh& mesh)
    {
        CommandBuffer<MemoryTransferCommand> memoryCommands;
        memoryCommands.emplace_back(MemoryTransferCommand{});

        MemoryTransferCommand& command = memoryCommands[0];
        command.Data = &mesh;
        command.MemoryTargetID = GPUResourceInspector::GetResourceID(&mesh);
        command.TargetType = MemoryTargetType::Mesh;
        command.TransferType = MemoryTransferType::Load;

        // Load and assign id

        RendererAPI->TransferData(memoryCommands);
        GPUResourceInspector::SetResourceID(&mesh, command.MemoryTargetID);
        GPUResourceInspector::ValidateResource(&mesh);

        return command.MemoryTargetID;
    }

    void RenderingSystem::FreeUnusedMeshes(const std::unordered_set<unsigned int>& usedMeshesIDs)
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

    void RenderingSystem::FreeMeshes(const std::unordered_set<unsigned int>& meshesToFree)
    {
        for (unsigned int meshID : meshesToFree)
        {
            FreeMesh(meshID);
        }
    }

    void RenderingSystem::FreeMesh(unsigned int meshID)
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

    void RenderingSystem::LoadMaterialsData(const Scene& scene, std::unordered_set<unsigned int>& outLoadedTexturesIDs, std::unordered_set<unsigned int>& outLoadedBuffersIDs)
    {
        for (const SceneObject& sceneObject : scene.SceneObjects)
        {
            std::vector<std::weak_ptr<Material>> materials = sceneObject.GetSceneObjectMaterials();

            for (std::weak_ptr<Material>& material : materials)
            {
                if (std::shared_ptr<Material> shMaterial = material.lock())
                    LoadMaterialData(*shMaterial, outLoadedTexturesIDs, outLoadedBuffersIDs);
            }
        }
    }

    void RenderingSystem::LoadMaterialData(Material& material, std::unordered_set<unsigned int>& outLoadedTextures, std::unordered_set<unsigned int>& outLoadedBuffers)
    {
        CommandBuffer<MemoryTransferCommand> commandBuffer; // TODO: send all with one request?

        // Load textures
        for (std::pair<const std::string, std::weak_ptr<Texture>>& textureData : material.TextureData)
        {
            if (std::shared_ptr<Texture> shTexture = textureData.second.lock())
            {
                LoadTextureAndCache(*shTexture);
                outLoadedTextures.insert(GPUResourceInspector::GetResourceID(shTexture.get()));
            }
        }

        // Load buffers
        for (std::pair<const std::string, std::weak_ptr<IBuffer>>& bufferData : material.BufferData)
        {
            if (std::shared_ptr<IBuffer> shBuffer = bufferData.second.lock())
            {
                LoadBufferAndCache(shBuffer.get());
                outLoadedBuffers.insert(GPUResourceInspector::GetResourceID(shBuffer.get()));
            }
        }
    }

    void RenderingSystem::LoadTextureAndCache(Texture& texture)
    {
        unsigned int textureID = GPUResourceInspector::GetResourceID(&texture);

        if (IsTextureCached(textureID) && texture.IsResourceValid())
        {
            return;
        }

        unsigned int newId = LoadTexture(texture);
        CacheTexture(newId);
    }

    unsigned int RenderingSystem::LoadTexture(Texture& texture)
    {
        CommandBuffer<MemoryTransferCommand> commandBuffer;
        commandBuffer.emplace_back(MemoryTransferCommand{});

        MemoryTransferCommand& command = commandBuffer[0];
        command.Data = (void*)&texture;
        command.MemoryTargetID = GPUResourceInspector::GetResourceID(&texture);
        command.TargetType = MemoryTargetType::Texture;
        command.TransferType = MemoryTransferType::Load;

        // Load, assign id and cache

        RendererAPI->TransferData(commandBuffer);
        GPUResourceInspector::SetResourceID(&texture, command.MemoryTargetID);
        GPUResourceInspector::ValidateResource(&texture);

        return command.MemoryTargetID;
    }

    void RenderingSystem::LoadBufferAndCache(IBuffer* buffer)
    {
        unsigned int bufferID = GPUResourceInspector::GetResourceID(buffer);

        if (IsBufferCached(bufferID) && buffer->IsResourceValid())
        {
            return;
        }

        unsigned int newId = LoadBuffer(buffer);
        CacheBuffer(newId);
    }

    unsigned int RenderingSystem::LoadBuffer(IBuffer* buffer)
    {
        CommandBuffer<MemoryTransferCommand> commandBuffer;
        commandBuffer.emplace_back(MemoryTransferCommand{});

        MemoryTransferCommand& command = commandBuffer[0];
        command.Data = buffer;
        command.MemoryTargetID = GPUResourceInspector::GetResourceID(buffer);
        command.TargetType = MemoryTargetType::Buffer;
        command.TransferType = MemoryTransferType::Load;

        // Load and assign id

        RendererAPI->TransferData(commandBuffer);
        GPUResourceInspector::SetResourceID(buffer, command.MemoryTargetID);
        GPUResourceInspector::ValidateResource(buffer);

        return command.MemoryTargetID;
    }

    void RenderingSystem::FreeUnusedTextures(const std::unordered_set<unsigned int>& usedTexturesIDs)
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

    void RenderingSystem::FreeTextures(const std::unordered_set<unsigned int>& texturesToFree)
    {
        for (unsigned int textureID : texturesToFree)
        {
            FreeTexture(textureID);
        }
    }

    void RenderingSystem::FreeTexture(unsigned int textureID)
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

    void RenderingSystem::FreeUnusedBuffers(const std::unordered_set<unsigned int>& usedBuffersIDs)
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

    void RenderingSystem::FreeBuffers(const std::unordered_set<unsigned int>& buffersToFree)
    {
        for (unsigned int bufferID : buffersToFree)
        {
            FreeBuffer(bufferID);
        }
    }

    void RenderingSystem::FreeBuffer(unsigned int bufferID)
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

    void RenderingSystem::LoadCameraData(const Camera& camera)
    {
        CameraBufferData data{};
        data.ViewMatrix = camera.GetViewMatrix();
        data.ProjectionMatrix = camera.GetProjectionMatrix();
        data.CameraPosition = camera.Transform.GetPosition();
        CameraBuffer.SetData(data, 0);

        LoadBuffer(&CameraBuffer);
    }

    void RenderingSystem::LoadTransformData(const Transform& transform)
    {
        TransformBufferData data{};
        data.ModelMatrix = transform.ToMatrix();
        data.NormalMatrix = glm::inverse(glm::transpose(data.ModelMatrix));
        TransformBuffer.SetData(data, 0);

        LoadBuffer(&TransformBuffer);
    }

    void RenderingSystem::LoadLightingData(const Scene& scene)
    {
        LightingBufferData data{};
        data.PointLightNumber = scene.PointLights.size();
        data.DirLightNumber = scene.DirectionalLights.size();
        LightingBuffer.SetData(data, 0);

        LoadBuffer(&LightingBuffer);
    }

    void RenderingSystem::LoadPointLightData(const std::vector<PointLight>& pointLights)
    {
        PointLightBuffer.ClearData();

        for (const PointLight& light : pointLights)
        {
            PointLightBufferData data{};
            data.Color = light.Color;
            data.Position = light.Transform.GetPosition();
            data.Intensity = light.Intensity;
            data.BulbSize = light.BulbSize;
            data.Radius = light.Radius;

            PointLightBuffer.AddData(data);
        }

        LoadBuffer(&PointLightBuffer);
    }

    void RenderingSystem::LoadDirLightData(const std::vector<DirectionalLight>& dirLights)
    {
        DirLightBuffer.ClearData();

        for (const DirectionalLight& light : dirLights)
        {
            DirLightBufferData data{};
            data.Color = light.Color;
            data.Position = light.Transform.GetPosition();
            data.Intensity = light.Intensity;
            data.Direction = light.GetLightDirection();

            DirLightBuffer.AddData(data);
        }

        LoadBuffer(&DirLightBuffer);
    }

    void RenderingSystem::SetRenderBufferTarget(const RenderBuffer& renderBuffer)
    {
        RendererAPI->BindRenderBuffer(GPUResourceInspector::GetResourceID(&renderBuffer));
    }

    void RenderingSystem::UnsetRenderBufferTarget(const RenderBuffer& renderBuffer)
    {
        RendererAPI->UnbindActiveRenderBuffer();
    }

    void RenderingSystem::SwapMainWindowBuffers()
    {
        RendererAPI->SwapWindowBuffers();
    }

    void RenderingSystem::BeforeDrawUI()
    {
        RendererAPI->BeforeDrawUI();
    }
}
