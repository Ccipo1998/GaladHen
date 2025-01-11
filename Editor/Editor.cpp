
#include "Editor.h"

#include <Renderer/Renderer.h>
#include <Renderer/Window.h>
#include <Renderer/Entities/RenderBuffer.h>
#include <Math/Ray.h>
#include <Math/Math.h>
#include <Editor/UI/Pages/MainPage.h>
#include "AssetsManager/AssetsManager.h"
#include <Renderer/Entities/Texture.h>
#include <Renderer/Entities/Material.h>
#include <Renderer/Entities/Model.h>

#include <glm/glm.hpp>
#include <imgui/imgui.h>

#include <string>

namespace GaladHen
{
    // STATIC INITIALIZATIONS -----------------------------------------------------------------------------------

    API Editor::API = API::OpenGL;
    Scene Editor::Scene{};
    Window Editor::Window{};
    UIPage* Editor::EditorUIPage{};

    // STATIC INITIALIZATIONS -----------------------------------------------------------------------------------

	void Editor::Init(GaladHen::API apiToUse)
	{
		// TODO: check if the editor was already initialized -> reload and reset to init state is required

		// make MainWindow
		Window.Init(apiToUse, "GaladHen");

        // init renderer
        Renderer::Init(apiToUse);

        // init scene for renderer;
        //EditorRenderer.LoadLightingData(EditorScene);
        //EditorRenderer.LoadCameraData(EditorScene.MainCamera);
        //EditorRenderer.LoadTransformData();

        // create main ui page
        EditorUIPage = new UIMainPage{ "MainPage", &Window };

        // Load and save pbr shader pipeline
        AssetsManager::LoadAndStoreShaderPipeline("../Shaders/pbr/Pbr.vert", "", "", "", "../Shaders/pbr/Pbr.frag", "", "PBR");

        AddDefaultGizmosToScene();
	}

    void Editor::AddDefaultBunnyToScene()
    {
        // load textures
        std::shared_ptr<Texture> texAlbedo = AssetsManager::LoadAndStoreTexture(
            "../Assets/Textures/StuccoRoughCast001_COL_2K_METALNESS.png",
            "StuccoAlbedo",
            TextureFormat::SRGB8);
        texAlbedo->SetNumberOfMipMaps(4);
        std::shared_ptr<Texture> texNormal = AssetsManager::LoadAndStoreTexture(
            "../Assets/Textures/StuccoRoughCast001_NRM_2K_METALNESS.png",
            "StuccoNormal",
            TextureFormat::RGB8);
        texNormal->SetNumberOfMipMaps(4);
        std::shared_ptr<Texture> texRoughness = AssetsManager::LoadAndStoreTexture(
            "../Assets/Textures/StuccoRoughCast001_ROUGHNESS_2K_METALNESS.png",
            "StuccoRoughness",
            TextureFormat::RGB);
        texRoughness->SetNumberOfMipMaps(4);

        // get pbr shader pipeline
        std::shared_ptr<ShaderPipeline> pbr = AssetsManager::GetShaderPipelineByName("PBR");

        // materials
        std::shared_ptr<Material> bunnyMat = std::shared_ptr<Material>{ new Material{ pbr } };
        bunnyMat->TextureData.emplace("DiffuseTexture", texAlbedo);
        bunnyMat->TextureData.emplace("NormalTexture", texNormal);
        bunnyMat->ScalarData.emplace("Metallic", 0.0f);
        bunnyMat->ScalarData.emplace("Roughness", 0.5f);
        bunnyMat->FunctionsData.emplace_back("DiffuseSampling");
        bunnyMat->FunctionsData.emplace_back("NormalSampling");
        bunnyMat->FunctionsData.emplace_back("MetallicConstant");
        bunnyMat->FunctionsData.emplace_back("RoughnessConstant");

        std::shared_ptr<Material> planeMat = std::shared_ptr<Material>{ new Material { pbr } };
        planeMat->Vec4Data.emplace("Diffuse", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
        bunnyMat->ScalarData.emplace("Metallic", 0.0f);
        bunnyMat->ScalarData.emplace("Roughness", 0.1f);
        bunnyMat->FunctionsData.emplace_back("DiffuseConstant");
        bunnyMat->FunctionsData.emplace_back("NormalInterpolated");
        bunnyMat->FunctionsData.emplace_back("MetallicConstant");
        bunnyMat->FunctionsData.emplace_back("RoughnessConstant");

        // load models
        std::shared_ptr<Model> bunny = AssetsManager::LoadAndStoreModel("../Assets/Models/bunny.glb", "Bunny");
        std::shared_ptr<Model> plane = AssetsManager::LoadAndStoreModel("../Assets/Models/plane.glb", "Plane");

        // bvh
        bunny->GetMeshes()[0].BVH.BuildBVH(bunny->GetMeshes()[0], AABBSplitMethod::PlaneCandidates);
        bunny->BVH.BuildBVH(*bunny, AABBSplitMethod::PlaneCandidates);

        // shaders and materials
        std::shared_ptr<ShaderPipeline> unlit = AssetsManager::LoadAndStoreShaderPipeline("../Shaders/Unlit/Unlit.vert", "", "", "", "../Shaders/Unlit/Unlit.frag", "", "Unlit");
        std::shared_ptr<Material> aabbMat = std::shared_ptr<Material>{ new Material { unlit } };
        aabbMat->Vec4Data.emplace("Diffuse", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        aabbMat->FunctionsData.emplace_back("DiffuseConstant");

        // ray
        Ray ray
        {
            Scene.MainCamera.Transform.GetPosition(),
            glm::vec3(0.0f, 0.0f, -1.0f),
            1000.0f
        };

        // load into scene
        SceneObject bunnyObj{ bunny };
        bunnyObj.SetMeshMaterialLink(0, bunnyMat);
        bunnyObj.Transform.SetPosition(glm::vec3(0.0f, 1.5f, 0.0f));
        bunnyObj.Transform.SetYaw(50.0f);
        Scene.SceneObjects.emplace_back(bunnyObj);

        SceneObject planeObj{ plane };
        planeObj.SetMeshMaterialLink(0, planeMat);
        planeObj.Transform.SetScale(glm::vec3(5.0f, 1.0f, 5.0f));
        //Scene.SceneObjects.emplace_back(planeObj);

        RayModelHitInfo hit = Math::RayModelIntersection(ray, *bunny, bunny->BVH, bunnyObj.Transform, BVHTraversalMethod::FrontToBack);
        std::vector<unsigned int> indices = { 0, 1, 2 };
        std::vector<MeshVertexData> vertices;
        vertices.push_back(bunny->GetMeshes()[0].GetVertices()[hit.VertexIndex0]);
        vertices[0].Position = glm::vec3(bunnyObj.Transform.ToMatrix() * glm::vec4(vertices[0].Position, 1.0f)) + glm::vec3(0.0f, 0.0f, 0.1f);
        vertices.push_back(bunny->GetMeshes()[0].GetVertices()[hit.VertexIndex1]);
        vertices[1].Position = glm::vec3(bunnyObj.Transform.ToMatrix() * glm::vec4(vertices[1].Position, 1.0f)) + glm::vec3(0.0f, 0.0f, 0.1f);
        vertices.push_back(bunny->GetMeshes()[0].GetVertices()[hit.VertexIndex2]);
        vertices[2].Position = glm::vec3(bunnyObj.Transform.ToMatrix() * glm::vec4(vertices[2].Position, 1.0f)) + glm::vec3(0.0f, 0.0f, 0.1f);
        Mesh aabbMesh{ vertices, indices, MeshPrimitive::Triangle };
        std::shared_ptr<Model> aabb = std::shared_ptr<Model>{ new Model { std::vector<Mesh>{ aabbMesh } } };
        
        SceneObject aabbObj{ aabb };
        aabbObj.SetMeshMaterialLink(0, aabbMat);
        Scene.SceneObjects.emplace_back(aabbObj);
    }

    void Editor::AddDefaultGizmosToScene()
    {
        // Gizmos
        std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5 };
        std::vector<MeshVertexData> vertices;
        MeshVertexData v{};
        v.Position = glm::vec3(0.0f);
        v.Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        vertices.push_back(v);
        v.Position = glm::vec3(1.0f, 0.0f, 0.0f);
        vertices.push_back(v);
        v.Color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        v.Position = glm::vec3(0.0f);
        vertices.push_back(v);
        v.Position = glm::vec3(0.0f, 1.0f, 0.0f);
        vertices.push_back(v);
        v.Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        v.Position = glm::vec3(0.0f);
        vertices.push_back(v);
        v.Position = glm::vec3(0.0f, 0.0f, 1.0f);
        vertices.push_back(v);
        Mesh gizmosMesh{ vertices, indices, MeshPrimitive::Line };
        std::shared_ptr<Model> gizmo{ new Model{ std::vector<Mesh>{ gizmosMesh } } };

        // shaders and materials
        std::shared_ptr<ShaderPipeline> unlit = AssetsManager::LoadAndStoreShaderPipeline("../Shaders/Unlit/Unlit.vert", "", "", "", "../Shaders/Unlit/Unlit.frag", "", "Unlit");
        std::shared_ptr<Material> aabbMat{ new Material{ unlit } };
        aabbMat->Vec4Data.emplace("Diffuse", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        aabbMat->FunctionsData.emplace_back("DiffuseConstant");

        std::shared_ptr<Material> gizmosMat{ new Material{ unlit } };
        gizmosMat->FunctionsData.emplace_back("VertexColorConstant");

        SceneObject gizmoObj{ gizmo };
        gizmoObj.SetMeshMaterialLink(0, gizmosMat);

        Scene.SceneObjects.emplace_back(gizmoObj);
    }

	void Editor::Run()
	{
        while (!Window.IsCloseWindowRequested())
        {
            Window.BeginFrame();

            glm::vec3 cameraMov = glm::vec3(0.0f);
            if (Window.IsKeyPressed(Input::KeyboardKey::W))
            {
                cameraMov.z += 1.0f;
            }
            if (Window.IsKeyPressed(Input::KeyboardKey::S))
            {
                cameraMov.z += -1.0f;
            }
            if (Window.IsKeyPressed(Input::KeyboardKey::D))
            {
                cameraMov.x += 1.0f;
            }
            if (Window.IsKeyPressed(Input::KeyboardKey::A))
            {
                cameraMov.x += -1.0f;
            }
            if (Window.IsKeyPressed(Input::KeyboardKey::E))
            {
                cameraMov.y += 1.0f;
            }
            if (Window.IsKeyPressed(Input::KeyboardKey::Q))
            {
                cameraMov.y += -1.0f;
            }
            glm::vec2 cameraRot = glm::vec2(0.0f);
            if (Window.IsKeyPressed(Input::MouseKey::Right))
            {
                Window.GetMousePositionDelta(cameraRot.x, cameraRot.y);
            }
            //Scene.MainCamera.ApplyCameraMovements(cameraMov, cameraRot, 0.0001f);
            //Renderer.UpdateCameraData(EditorScene.MainCamera);
            //renderer.Draw(aabbMesh, aabbMat);
            //CurrentRenderer.Draw(gizmos, gizmosMat);

            Renderer::Draw(Scene);

            /*EditorUIPage->NewFrame();
            EditorUIPage->BuildPage();
            EditorUIPage->Draw();*/

            Window.EndFrame();
        }

        Window.CloseWindow();
	}
}
