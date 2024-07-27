
#include <Renderer/Renderer.h>
#include <Renderer/Window.h>
#include <Core/Input.h>
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
    Renderer renderer{API::OpenGL};
    renderer.Init();
    // renderer settings
    renderer.EnableDepthTest(true);

    // make window
    Window window{API::OpenGL};
    window.SetColorBufferClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

    // input to link to the window
    Input input{};
    window.SetKeyboardCallback(&Input::KeyboardCallback, &input);
    window.SetMouseKeyCallback(&Input::MouseKeyCallback, &input);
    window.SetMousePositionCallback(&Input::MousePositionCallback, &input);
    window.SetClosingWindowCallback(&Input::ClosingWindowCallback, &input);

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
    ShaderPipeline pbr = AssetsManager::GetPipelinePBR();

    // materials
    Material bunnyMat{&pbr, ShadingMode::SmoothShading};
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

    while (!input.IsCloseWindowRequested())
    {
        window.BeginFrame();

        renderer.Draw(scene);

        window.EndFrame();
    }

    window.CloseWindow();

    return 0;
}
