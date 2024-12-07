
#include "Renderer.h"

#include <memory>
#include <unordered_set>

#include "LayerAPI/OpenGL/RendererGL.h"
#include "LayerAPI/OpenGL/MaterialDataGL.h"
#include "RenderBuffer.h"
#include "LayerAPI/IRendererAPI.h"
#include "CommandBuffer.h"

#include <Core/Mesh.h>
#include <Core/Model.h>
#include <Core/Scene.h>
#include <Core/SceneObject.h>
#include <Core/Model.h>
#include <Core/ShaderProgram.h>
#include <Core/Shader.h>
#include <Core/Material.h>
#include <Core/FileLoader.h>
#include <Core/Texture.h>

#include <Utils/Log.h>

#include <unordered_set>

#define GH_DEFAULT_RENDER_CLEAR_COLOR glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f }
#define GH_DEFAULT_RENDER_BUFFER_SIZE glm::uvec2{ 1920, 1080 }

namespace GaladHen
{
    namespace Renderer
    {
        namespace // renderer's private namespace
        {
            // RENDERER DATA -----------------------------------------------------------------------------------------------------------------------------------------

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

                // TODO: creare i buffers

                RenderContext(unsigned int width, unsigned int height, RenderContextType renderContextType)
                    : FrontBuffer(CreateRenderBuffer(width, height))
                    , BackBuffer(std::shared_ptr<RenderBuffer>{})
                    , RenderContextType(renderContextType)
                {
                    if (RenderContextType == RenderContextType::DoubleBuffering)
                        BackBuffer = CreateRenderBuffer(width, height);
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

            // Static Renderer Data
            API CurrentAPI = API::OpenGL;
            std::unique_ptr<IRendererAPI> RendererAPI = nullptr; // API-specific renderer for API-specific operations
            bool Initialized = false;

            // Rendering Data
            IdList<RenderContext> RenderContexts; // the RenderContext with id 1 is the default one
            std::unordered_set<unsigned int> LoadedMeshesCache; // cache of already loaded meshes -> for reloading, it requires that a mesh knows when it has been modified
            std::unordered_set<unsigned int> CompiledShadersCache; // cache of already compiled shaders -> for reloading, it requires that a shader knows when it has been modified

            CommandBuffer<RenderCommand> RenderCommandBuffer{};
            CommandBuffer<MemoryTransferCommand> MemoryCommandBuffer{};
            CommandBuffer<CompileCommand> CompileCommandBuffer{};

            // RENDERER FUNCTIONALITIES ------------------------------------------------------------------------------------------------------------------------------

            RenderContext& GetDefaultRenderContext()
            {
                return RenderContexts.GetObjectWithId(1);
            }

            void BeforeDraw(RenderContext& renderContext)
            {
                // Operations needed before drawing

                // Clear back render buffer
                ClearRenderBuffer(renderContext.GetBackBuffer(), GH_DEFAULT_RENDER_CLEAR_COLOR);
            }

            void AfterDraw(RenderContext& renderContext)
            {
                // Operations needed after drawing

                // Swap buffers
                renderContext.SwapBuffers();
            }

            bool IsCached(const Mesh& mesh)
            {
                if (LoadedMeshesCache.find(mesh.MeshID) != LoadedMeshesCache.end())
                {
                    return true;
                }

                return false;
            }

            void Cache(const Mesh& mesh)
            {
                LoadedMeshesCache.insert(mesh.MeshID);
            }

            void Uncache(const Mesh& mesh)
            {
                LoadedMeshesCache.erase(mesh.MeshID);
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

                // Create default RenderContext
                RenderContexts.AddWithId(RenderContext{ GH_DEFAULT_RENDER_BUFFER_SIZE.x, GH_DEFAULT_RENDER_BUFFER_SIZE.y, RenderContextType::DoubleBuffering });

                break;
            }
            }

            RendererAPI->Init();

            Initialized = true;
        }

        std::shared_ptr<RenderBuffer> CreateRenderBuffer(unsigned int width, unsigned int height)
        {
            // TODO
        }

        void ClearRenderBuffer(std::shared_ptr<RenderBuffer> renderBuffer, glm::vec4 clearColor)
        {
            // TODO
        }

        void Draw(const Scene& scene)
        {
            BeforeDraw(GetDefaultRenderContext());

            //for (SceneObject& sceneObj : scene.SceneObjects)
            //{
            //    Draw(sceneObj);
            //}

            AfterDraw(GetDefaultRenderContext());
        }

        void UpdateModel(const Model& model)
        {
            // TODO
        }

        void UpdateMesh(const Mesh& mesh)
        {
            // TODO
        }

        void FreeModel(const Model& model)
        {
            // TODO
        }

        void FreeMesh(const Mesh& mesh)
        {
            // TODO
        }



        void LoadModels(Scene& scene)
        {
            // Load mesh data
            for (SceneObject& sceneObj : scene.SceneObjects)
            {
                LoadModel(*sceneObj.GetSceneObjectModel());
            }
        }

        void FreeModels(Scene& scene)
        {
            // Free mesh data
            for (SceneObject& sceneObj : scene.SceneObjects)
            {
                Model* mod = sceneObj.GetSceneObjectModel();

                for (Mesh& mesh : mod->Meshes)
                {
                    FreeMesh(mesh);
                }
            }
        }

        void LoadLightingData(Scene& scene)
        {
            // Allocate and populate memory for lighting
            RendererAPI->LoadLightingData(scene.PointLights, scene.DirectionalLights);
        }

        void UpdateLightingData(Scene& scene)
        {
            RendererAPI->UpdateLightingData(scene.PointLights, scene.DirectionalLights);
        }

        void FreeLightingData(Scene& scene)
        {
            // Free lighting data
            RendererAPI->FreeLightingData();
        }

        void CompileShaders(Scene& scene)
        {
            // shaders already compiled
            std::unordered_set<unsigned int> compiled;

            for (SceneObject& sceneObj : scene.SceneObjects)
            {
                Model* mod = sceneObj.GetSceneObjectModel();

                for (unsigned int i = 0; i < mod->Meshes.size(); ++i)
                {
                    Material& mat = sceneObj.GetMaterial(i);
                    if (compiled.find(mat.MaterialShader->ShaderProgramID) == compiled.end())
                    {
                        compiled.insert(mat.MaterialShader->ShaderProgramID);
                        CompileShaderPipeline(*mat.MaterialShader);
                    }
                }
            }
        }

        void LoadMesh(Mesh& mesh)
        {
            RendererAPI->LoadMeshData(mesh);
        }

        void FreeMesh(Mesh& mesh)
        {
            RendererAPI->FreeMeshData(mesh);
        }

        void LoadModel(Model& model)
        {
            for (Mesh& mesh : model.Meshes)
            {
                LoadMesh(mesh);
            }
        }

        void FreeModel(Model& model)
        {
            for (Mesh& mesh : model.Meshes)
            {
                FreeMesh(mesh);
            }
        }

        void LoadTexture(Texture& texture)
        {
            RendererAPI->LoadTexture(texture);
        }

        void FreeTexture(Texture& texture)
        {
            RendererAPI->FreeTexture(texture);
        }

        void LoadMaterialData(Material& material)
        {
            RendererAPI->LoadMaterialData(material);
        }

        void LoadCameraData(Camera& camera)
        {
            RendererAPI->LoadCameraData(camera);
        }

        void UpdateCameraData(Camera& camera)
        {
            RendererAPI->UpdateCameraData(camera);
        }

        void LoadTransformData()
        {
            RendererAPI->LoadTransformData();
        }

        void UpdateTransformData(TransformQuat& transform)
        {
            RendererAPI->UpdateTransformData(transform);
        }

        void Draw(Mesh& mesh, Material& material)
        {
            RendererAPI->UpdateTransformData(TransformQuat{});
            RendererAPI->Draw(mesh, material);
        }

        void Draw(SceneObject& object)
        {
            Model* mod = object.GetSceneObjectModel();

            for (unsigned int i = 0; i < mod->Meshes.size(); ++i)
            {
                Material& mat = object.GetMaterial(i);
                RendererAPI->UpdateTransformData(object.Transform);
                RendererAPI->Draw(mod->Meshes[i], mat);
            }
        }

        bool CompileShaderPipeline(ShaderPipeline& program)
        {
            std::string vertPath, tessContPath, tessEvalPath, geomPath, fragPath; // For error log

            if (program.VertexShader)
            {
                vertPath = program.VertexShader->ShaderFilePath;
            }
            if (program.TessContShader)
            {
                tessContPath = program.TessContShader->ShaderFilePath;
            }
            if (program.TessEvalShader)
            {
                tessEvalPath = program.TessEvalShader->ShaderFilePath;
            }
            if (program.GeometryShader)
            {
                geomPath = program.GeometryShader->ShaderFilePath;
            }
            if (program.FragmentShader)
            {
                fragPath = program.FragmentShader->ShaderFilePath;
            }

            CompilationResult result = RendererAPI->CompileShaderPipeline(program);

            if (!result.Success())
            {
                std::string error;

                if (result.linkSuccess)
                {
                    error = "Compilation error for shader pipeline:\n";
                }
                else
                {
                    error = "Linking error in shader pipeline:\n";
                }

                if (!result.vSuccess)
                {
                    error.append("ERROR in vertex shader ");
                    error.append(vertPath);
                    error.append(": ");
                    error.append(result.vLog);
                    error.append("\n");
                }
                if (!result.tcSuccess)
                {
                    error.append("ERROR in tesselation control shader ");
                    error.append(tessContPath);
                    error.append(": ");
                    error.append(result.tcLog);
                    error.append("\n");
                }
                if (!result.teSuccess)
                {
                    error.append("ERROR in tesselation evaluation shader ");
                    error.append(tessEvalPath);
                    error.append(": ");
                    error.append(result.teLog);
                    error.append("\n");
                }
                if (!result.gSuccess)
                {
                    error.append("ERROR in geomerty shader ");
                    error.append(geomPath);
                    error.append(": ");
                    error.append(result.gLog);
                    error.append("\n");
                }
                if (!result.fSuccess)
                {
                    error.append("ERROR in fragment shader ");
                    error.append(fragPath);
                    error.append(": ");
                    error.append(result.fLog);
                    error.append("\n");
                }

                if (!result.linkSuccess)
                {
                    error.append(result.linkLog);
                }

                Log::Error("Renderer", error);
            }

            return result.Success();
        }

        bool CompileComputeShader(ComputeShader& program)
        {
            // Read files

            std::string computePath; // For error log

            if (program.CompShader)
            {
                computePath = program.CompShader->ShaderFilePath;
            }

            CompilationResult result = RendererAPI->CompileComputeShader(program);

            if (!result.Success())
            {
                std::string error;

                if (result.linkSuccess)
                {
                    error = "Compilation error for compute shader:\n";
                }
                else
                {
                    error = "Linking error for compute shader:\n";
                }

                error.append("ERROR in compute shader ");
                error.append(computePath);
                error.append(": ");
                error.append(result.cLog);
                error.append("\n");

                Log::Error("Renderer", error);
            }

            return result.Success();
        }

        const std::shared_ptr<RenderBuffer> GetRenderBuffer()
        {
            return RenderBuffer{ RendererAPI->GetRenderBuffer() };
        }

        void SetViewport(const glm::uvec2& position, const glm::uvec2& size)
        {
            RendererAPI->SetViewport(position, size);
        }

        void SetRenderTargetSize(const glm::uvec2& size)
        {
            RendererAPI->SetRenderTargetSize(size);
        }
    }
}
