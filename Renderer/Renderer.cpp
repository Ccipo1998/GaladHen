
#include "Renderer.h"

#include <Renderer/LayerAPI/OpenGL/RendererGL.h>

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

    void Renderer::CreateLowLevelMesh(Mesh& mesh)
    {

    }

    void Renderer::DestroyLowLevelMesh(unsigned int meshID)
    {

    }

    void Renderer::SetColorBufferClearColor(const glm::vec4 color)
    {

    }

    void Renderer::EnableDepthTest(bool enable)
    {
        
    }
}
