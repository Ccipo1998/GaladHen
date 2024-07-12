
#include "Renderer.h"

#include <Renderer/LayerAPI/OpenGL/RendererGL.h>
#include <GaladHen/Mesh.h>

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

    void Renderer::SendMeshDataToGPU(Mesh& mesh)
    {
        unsigned int meshID = RendererAPI->CreateLowLevelMesh();
        mesh.MeshID = meshID;

        RendererAPI->LoadMeshDataIntoGPU(mesh.Vertices, mesh.Indices, meshID);
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
