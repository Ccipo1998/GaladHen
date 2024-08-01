
#include <Renderer/Renderer.h>
#include <Renderer/Window.h>
#include <Core/Scene.h>
#include <Core/ShaderProgram.h>
#include <Core/Shader.h>
#include <Core/AssetsManager.h>
#include <Core/Material.h>
#include <Core/Texture.h>

#include <glm/glm.hpp>

#include <string>

using namespace GaladHen;

int main()
{
    // create opengl renderer
    Renderer renderer{ API::OpenGL };
    renderer.Init();
    // renderer settings
    renderer.EnableDepthTest(true);

    // make window
    Window window{ API::OpenGL, "GaladHen" };
    window.SetColorBufferClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

    // create scene
    Scene scene{};

    // load textures
    Texture* texAlbedo = AssetsManager::LoadAndStoreTexture(
        std::string{"../Assets/Textures/StuccoRoughCast001_COL_2K_METALNESS.png"},
        std::string{"StuccoAlbedo"},
        TextureFormat::RGB);
    Texture* texNormal = AssetsManager::LoadAndStoreTexture(
        std::string{"../Assets/Textures/StuccoRoughCast001_NRM_2K_METALNESS.png"},
        std::string{"StuccoNormal"},
        TextureFormat::SRGB);
    Texture* texRoughness = AssetsManager::LoadAndStoreTexture(
        std::string{"../Assets/Textures/StuccoRoughCast001_ROUGHNESS_2K_METALNESS.png"},
        std::string{"StuccoRoughness"},
        TextureFormat::RGB);

    // get pbr shader pipeline
    //ShaderPipeline pbr = AssetsManager::GetPipelinePBR();
    Shader vertex{ "../Shaders/test/VertexShader.vert", ShaderStage::Vertex };
    Shader fragment{ "../Shaders/test/FragmentShader.frag", ShaderStage::Fragment };
    ShaderPipeline test{};
    test.VertexShader = &vertex;
    test.FragmentShader = &fragment;

    // materials
    Material bunnyMat{&test, ShadingMode::SmoothShading};
    PBRMaterialData bunnyMatData{};
    bunnyMat.Data = &bunnyMatData;

    // load models
    Model* bunny = AssetsManager::LoadAndStoreModel(std::string("../Assets/Models/bunny.glb"), "Bunny");

    // load into scene
    std::vector<Material*> bunnyMats;
    bunnyMats.push_back(&bunnyMat);
    SceneObject objBunny{ bunny, bunnyMats };
    scene.SceneObjects.push_back(objBunny);

    // init scene for renderer
    renderer.LoadModels(scene);
    renderer.LoadLightingData(scene);
    renderer.CompileShaders(scene);
    renderer.LoadCameraData(scene.MainCamera);

    while (!window.IsCloseWindowRequested())
    {
        window.BeginFrame();

        glm::vec3 cameraMov = glm::vec3(0.0f);
        if (window.IsKeyPressed(KeyboardKey::W))
        {
            cameraMov.z += 1.0f;
        }
        if (window.IsKeyPressed(KeyboardKey::S))
        {
            cameraMov.z += -1.0f;
        }
        if (window.IsKeyPressed(KeyboardKey::D))
        {
            cameraMov.x += 1.0f;
        }
        if (window.IsKeyPressed(KeyboardKey::A))
        {
            cameraMov.x += -1.0f;
        }
        if (window.IsKeyPressed(KeyboardKey::E))
        {
            cameraMov.y += 1.0f;
        }
        if (window.IsKeyPressed(KeyboardKey::Q))
        {
            cameraMov.y += -1.0f;
        }
        glm::vec2 cameraRot = glm::vec2(0.0f);
        if (window.IsKeyPressed(MouseKey::Right))
        {
            window.GetMousePositionDelta(cameraRot.x, cameraRot.y);
        }

        scene.MainCamera.ApplyCameraMovements(cameraMov, cameraRot, 0.0001f);
        renderer.LoadCameraData(scene.MainCamera);

        renderer.Draw(scene);

        window.EndFrame();
    }

    window.CloseWindow();

    return 0;
}
