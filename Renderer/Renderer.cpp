
#include "Renderer.h"

#include <Renderer/LayerAPI/OpenGL/RendererGL.h>
#include <GaladHen/Mesh.h>
#include <GaladHen/Scene.h>
#include <GaladHen/SceneObject.h>
#include <GaladHen/Model.h>

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

            for (Mesh& mesh : mod->Meshes)
            {
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

    void Renderer::EnableDepthTest(bool enable)
    {
        RendererAPI->EnableDepthTest(enable);
    }
}
