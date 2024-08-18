
#include "Renderer.h"
#include "LayerAPI/OpenGL/RendererGL.h"
#include "LayerAPI/OpenGL/MaterialDataGL.h"

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

namespace GaladHen
{
    Renderer::Renderer(API apiToUse)
        : CurrentAPI(apiToUse)
        , RendererAPI(nullptr)
    {
        // Initialization basing on API

        switch (apiToUse)
        {
        case API::OpenGL:

            // Create low level OpenGL Renderer
            RendererAPI = new RendererGL{};

            break;

        default:
            break;
        }
    }

    void Renderer::Init()
    {
        RendererAPI->Init();
    }

    void Renderer::LoadModels(Scene& scene)
    {
        // Load mesh data
        for (SceneObject& sceneObj : scene.SceneObjects)
        {
            LoadModel(*sceneObj.GetSceneObjectModel());
        }
    }

    void Renderer::FreeModels(Scene& scene)
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

    void Renderer::LoadLightingData(Scene& scene)
    {
        // Allocate and populate memory for lighting
        RendererAPI->LoadLightingData(scene.PointLights, scene.DirectionalLights);
    }

    void Renderer::UpdateLightingData(Scene& scene)
    {
        RendererAPI->UpdateLightingData(scene.PointLights, scene.DirectionalLights);
    }

    void Renderer::FreeLightingData(Scene& scene)
    {
        // Free lighting data
        RendererAPI->FreeLightingData();
    }

    void Renderer::CompileShaders(Scene& scene)
    {
        // shaders already compiled
        std::unordered_set<unsigned int> compiled;

        for (SceneObject& sceneObj : scene.SceneObjects)
        {
            Model* mod = sceneObj.GetSceneObjectModel();

            for (unsigned int i = 0; i < mod->Meshes.size(); ++i)
            {
                if (Material* mat = sceneObj.GetMaterial(i))
                {
                    if (compiled.find(mat->MaterialShader->ShaderProgramID) == compiled.end())
                    {
                        compiled.insert(mat->MaterialShader->ShaderProgramID);
                        CompileShaderPipeline(*mat->MaterialShader);
                    }
                }
            }
        }
    }

    void Renderer::LoadMesh(Mesh& mesh)
    {
        RendererAPI->LoadMeshData(mesh);
    }

    void Renderer::FreeMesh(Mesh& mesh)
    {
        RendererAPI->FreeMeshData(mesh);
    }

    void Renderer::LoadModel(Model& model)
    {
        for (Mesh& mesh : model.Meshes)
        {
            LoadMesh(mesh);
        }
    }

    void Renderer::FreeModel(Model& model)
    {
        for (Mesh& mesh : model.Meshes)
        {
            FreeMesh(mesh);
        }
    }

    void Renderer::LoadTexture(Texture& texture)
    {
        RendererAPI->LoadTexture(texture);
    }

    void Renderer::FreeTexture(Texture& texture)
    {
        RendererAPI->FreeTexture(texture);
    }

    void Renderer::LoadMaterialData(Material& material)
    {
        RendererAPI->LoadMaterialData(material);
    }

    void Renderer::LoadCameraData(Camera& camera)
    {
        RendererAPI->LoadCameraData(camera);
    }

    void Renderer::UpdateCameraData(Camera& camera)
    {
        RendererAPI->UpdateCameraData(camera);
    }

    void Renderer::Draw(Scene& scene)
    {
        for (SceneObject& sceneObj : scene.SceneObjects)
        {
            Model* mod = sceneObj.GetSceneObjectModel();

            for (unsigned int i = 0; i < mod->Meshes.size(); ++i)
            {
                if (Material* mat = sceneObj.GetMaterial(i))
                {
                    RendererAPI->Draw(mod->Meshes[i], *mat);
                }
            }
        }
    }

    bool Renderer::CompileShaderPipeline(ShaderPipeline& program)
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

    bool Renderer::CompileComputeShader(ComputeShader& program)
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
}
