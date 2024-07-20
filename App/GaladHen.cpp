
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

    // load shaders
    Shader pbrVertex{std::string{"../Shaders/pbr/Pbr.vert"}, ShaderStage::Vertex};
    Shader pbrFragment{std::string{"../Shaders/pbr/Pbr.frag"}, ShaderStage::Fragment};
    ShaderProgram pbr{&pbrVertex, nullptr, nullptr, &pbrFragment};

    // load textures
    TextureImage* texImgAlbedo = AssetsManager::LoadAndStoreTexImage(
        std::string{"../Assets/Textures/StuccoRoughCast001_COL_2K_METALNESS.png"},
        std::string{"StuccoAlbedo"});
    TextureImage* texImgNormal = AssetsManager::LoadAndStoreTexImage(
        std::string{"../Assets/Textures/StuccoRoughCast001_NRM_2K_METALNESS.png"},
        std::string{"StuccoNormal"});
    TextureImage* texImgRoughness = AssetsManager::LoadAndStoreTexImage(
        std::string{"../Assets/Textures/StuccoRoughCast001_ROUGHNESS_2K_METALNESS.png"},
        std::string{"StuccoRoughness"});

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
    renderer.InitScene(scene);

    while (!input.IsCloseWindowRequested())
    {
        window.BeginFrame();

        renderer.Draw(scene);

        window.EndFrame();
    }

    window.CloseWindow();

    return 0;
}
