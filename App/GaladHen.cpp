
#include <Renderer/Renderer.h>
#include <Renderer/Window.h>
#include <Core/Scene.h>
#include <Core/ShaderProgram.h>
#include <Core/Shader.h>
#include <Core/AssetsManager.h>
#include <Core/Model.h>
#include <Core/Material.h>
#include <Core/Texture.h>

#include <Core/BVH/BVH.h>
#include <Core/BVH/BVHNode.h>
#include <Core/AABB/AABB.h>

#include <glm/glm.hpp>

#include <string>

using namespace GaladHen;

int main()
{
    // create opengl renderer
    Renderer renderer{ API::OpenGL };
    renderer.Init();

    // make window
    Window window{ API::OpenGL, "GaladHen" };
    window.SetColorBufferClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    window.EnableDepthTest(true);

    // create scene
    Scene scene{};

    // load textures
    Texture* texAlbedo = AssetsManager::LoadAndStoreTexture(
        "../Assets/Textures/StuccoRoughCast001_COL_2K_METALNESS.png",
        "StuccoAlbedo",
        TextureFormat::SRGB8);
    texAlbedo->NumberOfMipMaps = 4;
    Texture* texNormal = AssetsManager::LoadAndStoreTexture(
        "../Assets/Textures/StuccoRoughCast001_NRM_2K_METALNESS.png",
        "StuccoNormal",
        TextureFormat::RGB8);
    texNormal->NumberOfMipMaps = 4;
    Texture* texRoughness = AssetsManager::LoadAndStoreTexture(
        "../Assets/Textures/StuccoRoughCast001_ROUGHNESS_2K_METALNESS.png",
        "StuccoRoughness",
        TextureFormat::RGB);
    texRoughness->NumberOfMipMaps = 4;

    // get pbr shader pipeline
    ShaderPipeline pbr = AssetsManager::GetPipelinePBR();
    /*Shader vertex{ "../Shaders/pbr/Pbr.vert", ShaderStage::Vertex };
    Shader fragment{ "../Shaders/pbr/Pbr.frag", ShaderStage::Fragment };
    ShaderPipeline test{};
    test.VertexShader = &vertex;
    test.FragmentShader = &fragment;*/

    // materials
    Material bunnyMat{ &pbr, ShadingMode::SmoothShading };
    PBRMaterialData bunnyMatData{};
    TextureParameters diffuse{};
    diffuse.TextureSource = texAlbedo;
    TextureParameters normal{};
    normal.TextureSource = texNormal;
    bunnyMatData.Metallic = 0.0f;
    bunnyMatData.Roughness = 0.5f;
    /*TextureParameters roughness{};
    roughness.TextureSource = texRoughness;*/
    bunnyMatData.DiffuseTexture = diffuse;
    bunnyMatData.Diffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    bunnyMatData.NormalTexture = normal;
    //bunnyMatData.RoughnessTexture = roughness;
    bunnyMat.Data = &bunnyMatData;

    Material planeMat{ &pbr, ShadingMode::SmoothShading };
    PBRMaterialData planeMatData{};
    planeMatData.Diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    planeMatData.Roughness = 0.1f;
    planeMatData.Metallic = 0.0f;
    planeMat.Data = &planeMatData;

    // load models
    Model* bunny = AssetsManager::LoadAndStoreModel("../Assets/Models/bunny.glb", "Bunny");
    Model* plane = AssetsManager::LoadAndStoreModel("../Assets/Models/plane.glb", "Plane");

    // bvh
    BVH bunnyBVH{};
    bunnyBVH.BuildBVH_InPlace(bunny->Meshes[0]);
    //bunnyBVH.GetRootNode().AABoundingBox.UpdateAABB(objBunny.Transform);
    Mesh aabbMesh = bunnyBVH.GetNode(0).AABoundingBox.ToMesh();
    Shader vUnlit{ "../Shaders/Unlit/Unlit.vert", ShaderStage::Vertex };
    Shader fUnlit{ "../Shaders/Unlit/Unlit.frag", ShaderStage::Fragment };
    ShaderPipeline unlit{};
    unlit.VertexShader = &vUnlit;
    unlit.FragmentShader = &fUnlit;
    Material aabbMat{ &unlit, ShadingMode::SmoothShading };
    UnlitMaterialData unlitData{};
    unlitData.DiffuseColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    aabbMat.Data = &unlitData;
    renderer.LoadMesh(aabbMesh);
    renderer.CompileShaderPipeline(unlit);

    // load into scene
    std::vector<Material*> bunnyMats;
    bunnyMats.push_back(&bunnyMat);
    SceneObject objBunny{ bunny, bunnyMats };
    objBunny.Transform.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    scene.SceneObjects.push_back(objBunny);
    std::vector<Material*> planeMats;
    planeMats.push_back(&planeMat);
    SceneObject objPlane{ plane, planeMats };
    objPlane.Transform.SetScale(glm::vec3(5.0f, 1.0f, 5.0f));
    //scene.SceneObjects.push_back(objPlane);

    // init scene for renderer
    renderer.LoadModels(scene);
    renderer.LoadLightingData(scene);
    renderer.CompileShaders(scene);
    renderer.LoadCameraData(scene.MainCamera);
    renderer.LoadTexture(*texAlbedo);
    renderer.LoadTexture(*texNormal);
    renderer.LoadTexture(*texRoughness);
    renderer.LoadTransformData();

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
        renderer.UpdateCameraData(scene.MainCamera);

        renderer.Draw(scene);
        renderer.Draw(aabbMesh, aabbMat);

        window.EndFrame();
    }

    window.CloseWindow();

    return 0;
}
