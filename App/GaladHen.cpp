
#include <Renderer/Renderer.h>
#include <Renderer/Window.h>
#include <GaladHen/Input.h>
#include <GaladHen/Scene.h>
#include <GaladHen/ShaderProgram.h>
#include <GaladHen/Shader.h>
#include <GaladHen/AssetsManager.h>
#include <GaladHen/Material.h>

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
    Window window{API::OpenGL}; // temp: we need to retrieve the API
    window.SetColorBufferClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

    // input to link to the window
    Input input{};
    window.SetKeyboardCallback(&Input::KeyboardCallback, &input);
    window.SetMouseKeyCallback(&Input::MouseKeyCallback, &input);
    window.SetMousePositionCallback(&Input::MousePositionCallback, &input);

    // create scene
    Scene scene{};

    // load shaders
    Shader pbrVertex{std::string{"shaders/pbr/Pbr.vert"}, ShaderStage::Vertex};
    Shader pbrFragment{std::string{"shaders/pbr/Pbr.frag"}, ShaderStage::Fragment};
    ShaderProgram pbr{&pbrVertex, nullptr, nullptr, &pbrFragment};

    // load textures
    TextureImage* texImgAlbedo = AssetsManager::LoadAndStoreTexImage(
        std::string{"textures/StuccoRoughCast001_COL_2K_METALNESS.png"},
        std::string{"StuccoAlbedo"});
    TextureImage* texImgNormal = AssetsManager::LoadAndStoreTexImage(
        std::string{"textures/StuccoRoughCast001_NRM_2K_METALNESS.png"},
        std::string{"StuccoNormal"});
    TextureImage* texImgRoughness = AssetsManager::LoadAndStoreTexImage(
        std::string{"textures/StuccoRoughCast001_ROUGHNESS_2K_METALNESS.png"},
        std::string{"StuccoRoughness"});

    // materials
    Material bunnyMat{&pbr, ShadingMode::SmoothShading};
    PBRMaterialData bunnyMatData{};
    bunnyMat.Data = &bunnyMatData;

    // load models
    Model* bunny = AssetsManager::LoadAndStoreModel(std::string("prefabs/bunny.glb"), "Bunny");

    while (true)
    {
        window.BeginFrame();

        window.EndFrame();
    }

    return 0;
}
