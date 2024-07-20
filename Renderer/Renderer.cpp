
#include "Renderer.h"

#include <Renderer/LayerAPI/OpenGL/RendererGL.h>
#include <GaladHen/Mesh.h>
#include <GaladHen/Scene.h>
#include <GaladHen/SceneObject.h>
#include <GaladHen/Model.h>
#include <GaladHen/ShaderProgram.h>
#include <GaladHen/Shader.h>
#include <GaladHen/Material.h>

#include <Utils/Log.h>
#include <Utils/FileLoader.h>

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

    void Renderer::Draw(Scene& scene)
    {
        
    }

    void Renderer::InitScene(Scene& scene)
    {
        // Load mesh data
        for (SceneObject& sceneObj : scene.SceneObjects)
        {
            Model* mod = sceneObj.GetSceneObjectModel();

            for (unsigned int i = 0; i < mod->Meshes.size(); ++i)
            {
                // mesh data
                LoadMeshDataIntoGPU(mod->Meshes[i]);

                // shader
                if (Material* mat = sceneObj.GetMaterial(i))
                {
                    CompileShaderPipeline(*mat->MaterialShader);
                }
            }

            for (Mesh& mesh : mod->Meshes)
            {
                // mesh data
                LoadMeshDataIntoGPU(mesh);
            }
        }

        // Allocate and populate memory for lighting
        RendererAPI->LoadLighingDataIntoGPU(scene.PointLights, scene.DirectionalLights);
    }

    void Renderer::UpdateFromScene(Scene& scene, SceneStatus& status)
    {
        // TODO
    }

    void Renderer::ClearScene(Scene& scene)
    {
        // Clear mesh data
        for (SceneObject& sceneObj : scene.SceneObjects)
        {
            Model* mod = sceneObj.GetSceneObjectModel();

            for (Mesh& mesh : mod->Meshes)
            {
                FreeMeshDataFromGPU(mesh);
            }
        }

        // Free lighting data
        RendererAPI->FreeLightingDataFromGPU();
    }

    void Renderer::LoadMeshDataIntoGPU(Mesh& mesh)
    {
        if (mesh.MeshID == 0)
        {
            // New mesh -> create low level mesh
            mesh.MeshID = RendererAPI->CreateLowLevelMesh();
        }

        RendererAPI->LoadMeshDataIntoGPU(mesh.Vertices, mesh.Indices, mesh.MeshID);
    }

    void Renderer::FreeMeshDataFromGPU(Mesh& mesh)
    {
        RendererAPI->DestroyLowLevelMesh(mesh.MeshID);
    }

    bool Renderer::CompileShaderPipeline(ShaderPipeline& program)
    {
        if (program.ShaderProgramID == 0)
        {
            // New shader program -> create low level shader program
            program.ShaderProgramID = RendererAPI->CreateLowLevelShaderProgram();
        }

        // Read files
        
        std::string vertex, tessCont, tessEval, geometry, fragment;
        std::string vertPath, tessContPath, tessEvalPath, geomPath, fragPath; // For error log

        if (program.VertexShader)
        {
            vertPath = program.VertexShader->ShaderFilePath;
            vertex = FileLoader::ReadTextFile(vertPath.data());
        }
        if (program.TessContShader)
        {
            tessContPath = program.TessContShader->ShaderFilePath;
            tessCont = FileLoader::ReadTextFile(tessContPath.data());
        }
        if (program.TessEvalShader)
        {
            tessEvalPath = program.TessEvalShader->ShaderFilePath;
            tessEval = FileLoader::ReadTextFile(tessEvalPath.data());
        }
        if (program.GeometryShader)
        {
            geomPath = program.GeometryShader->ShaderFilePath;
            geometry = FileLoader::ReadTextFile(geomPath.data());
        }
        if (program.FragmentShader)
        {
            fragPath = program.FragmentShader->ShaderFilePath;
            fragment = FileLoader::ReadTextFile(fragPath.data());
        }

        CompilationResult result = RendererAPI->CompileShaderProgramPipeline(vertex, tessCont, tessEval, geometry, fragment, program.ShaderProgramID);

        if (!result.Success())
        {
            std::string error;

            if (result.linkSuccess)
            {
                error = "Compilation error for shader pipeline:\n";
            }
            else
            {
                error = "Linking error for shader pipeline:\n";
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

            Log::Error("Renderer", error);
        }

        return result.Success();
    }

    void Renderer::EnableDepthTest(bool enable)
    {
        RendererAPI->EnableDepthTest(enable);
    }
}
