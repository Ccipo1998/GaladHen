
#include "Editor.h"

#include <Systems/RenderingSystem/RenderingSystem.h>
#include <Systems/RenderingSystem/Entities/RenderBuffer.h>
#include <Systems/InputSystem/InputSystem.h>
#include <Systems/InputSystem/Input.h>
#include <Math/Ray.h>
#include <Math/Math.h>
#include <Editor/UI/Pages/MainPage.h>
#include <Systems/RenderingSystem/Entities/Texture.h>
#include <Systems/RenderingSystem/Entities/Material.h>
#include <Systems/RenderingSystem/Entities/Model.h>
#include <Systems/RenderingSystem/Entities/ShaderPipeline.h>
#include <Systems/AssetSystem/AssetSystem.h>
#include <Systems/TimeSystem/TimeSystem.h>

#include <glm/glm.hpp>


namespace GaladHen
{
    // STATIC INITIALIZATIONS -----------------------------------------------------------------------------------

    Scene Editor::Scene{};

    // STATIC INITIALIZATIONS -----------------------------------------------------------------------------------

	void Editor::Init(GaladHen::API apiToUse)
	{
		// make MainWindow
        RenderingSystem::GetInstance()->CreateMainWindow("GaladHen", glm::uvec2(1280, 720));
        RenderingSystem::GetInstance()->SetUIPage<UIMainPage>("MainPage");

        // link input callback
        InputSystem::GetInstance()->RegisterToInputProvider();

        //AddDefaultGizmosToScene();
        AddDefaultBunnyToScene();
	}

    void Editor::AddDefaultBunnyToScene()
    {
        std::weak_ptr<ShaderPipeline> pbrBunny = AssetSystem::GetInstance()->LoadAndStoreShaderPipeline("GaladHen/Shaders/ShadingModels/Pbr/Pbr.vert", "", "", "", "GaladHen/Shaders/Materials/Bunny.frag", "", "Bunny");

        // load textures
        std::weak_ptr<Texture> texAlbedo = AssetSystem::GetInstance()->LoadAndStoreTexture(
            "Assets/Textures/StuccoRoughCast001_COL_2K_METALNESS.png",
            "StuccoAlbedo",
            TextureFormat::SRGB8);
        texAlbedo.lock()->SetNumberOfMipMaps(4);
        std::weak_ptr<Texture> texNormal = AssetSystem::GetInstance()->LoadAndStoreTexture(
            "Assets/Textures/StuccoRoughCast001_NRM_2K_METALNESS.png",
            "StuccoNormal",
            TextureFormat::RGB8);
        texNormal.lock()->SetNumberOfMipMaps(4);
        std::weak_ptr<Texture> texRoughness = AssetSystem::GetInstance()->LoadAndStoreTexture(
            "Assets/Textures/StuccoRoughCast001_ROUGHNESS_2K_METALNESS.png",
            "StuccoRoughness",
            TextureFormat::RG8);
        texRoughness.lock()->SetNumberOfMipMaps(4);

        std::weak_ptr<ShaderPipeline> unlit = AssetSystem::GetInstance()->LoadAndStoreShaderPipeline("GaladHen/Shaders/ShadingModels/Unlit/Unlit.vert", "", "", "", "GaladHen/Shaders/Materials/UnlitColor.frag", "", "UnlitColor");
        //std::weak_ptr<Material> aabbMat = AssetSystem::GetInstance()->CreateAndStoreMaterial("AABBMaterial");
        //std::shared_ptr<Material> shAabbMat = aabbMat.lock();
        //// no need to check ptr validity
        //shAabbMat->SetPipeline(unlit);
        //shAabbMat->Vec4Data.emplace("Diffuse", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        // materials
        std::weak_ptr<Material> bunnyMat = AssetSystem::GetInstance()->CreateAndStoreMaterial("BunnyMaterial");
        std::shared_ptr<Material> shBunnyMat = bunnyMat.lock();
        shBunnyMat->SetPipeline(pbrBunny);
        shBunnyMat->TextureData.emplace("DiffuseTexture", texAlbedo);
        shBunnyMat->TextureData.emplace("NormalMap", texNormal);
        shBunnyMat->ScalarData.emplace("Metallic", 0.0f);
        shBunnyMat->ScalarData.emplace("Roughness", 0.5f);

        std::weak_ptr<Material> planeMat = AssetSystem::GetInstance()->CreateAndStoreMaterial("PlaneMaterial");
        std::shared_ptr<Material> shPlaneMat = planeMat.lock();
        shPlaneMat->SetPipeline(unlit);
        shPlaneMat->Vec4Data.emplace("ConstantColor", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
        shPlaneMat->ScalarData.emplace("Metallic", 0.0f);
        shPlaneMat->ScalarData.emplace("Roughness", 0.1f);

        // load models
        std::weak_ptr<Model> bunny = AssetSystem::GetInstance()->LoadAndStoreModel("Assets/Models/bunny.glb", "Bunny");
        std::weak_ptr<Model> plane = AssetSystem::GetInstance()->LoadAndStoreModel("Assets/Models/plane.glb", "Plane");

        // bvh
        /*bunny->GetMeshes()[0].BVH.BuildBVH(bunny->GetMeshes()[0], AABBSplitMethod::PlaneCandidates);
        bunny->BVH.BuildBVH(*bunny, AABBSplitMethod::PlaneCandidates);*/

        // shaders and materials
        //std::shared_ptr<ShaderPipeline> unlit = AssetsManager::LoadAndStoreShaderPipeline("GaladHen/Shaders/ShadingModels/Unlit/Unlit.vert", "", "", "", "GaladHen/Shaders/Materials/UnlitColor.frag", "", "UnlitColor");
        //std::shared_ptr<Material> aabbMat = std::shared_ptr<Material>{ new Material { unlit } };
        //aabbMat->Vec4Data.emplace("ConstantColor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        //// ray
        //Ray ray
        //{
        //    Scene.MainCamera.Transform.GetPosition(),
        //    glm::vec3(0.0f, 0.0f, -1.0f),
        //    1000.0f
        //};

        // load into scene
        SceneObject bunnyObj{ bunny };
        bunnyObj.SetMeshMaterialLink(0, bunnyMat);
        bunnyObj.Transform.SetPosition(glm::vec3(0.0f, 1.5f, 0.0f));
        bunnyObj.Transform.SetYaw(50.0f);
        Scene.SceneObjects.emplace_back(bunnyObj);

        SceneObject planeObj{ plane };
        planeObj.SetMeshMaterialLink(0, planeMat);
        planeObj.Transform.SetScale(glm::vec3(10.0f, 1.0f, 10.0f));
        Scene.SceneObjects.emplace_back(planeObj);

        //RayModelHitInfo hit = Math::RayModelIntersection(ray, *bunny, bunny->BVH, bunnyObj.Transform, BVHTraversalMethod::FrontToBack);
        //std::vector<unsigned int> indices = { 0, 1, 2 };
        //std::vector<MeshVertexData> vertices;
        //vertices.push_back(bunny->GetMeshes()[0].GetVertices()[hit.VertexIndex0]);
        //vertices[0].Position = glm::vec3(bunnyObj.Transform.ToMatrix() * glm::vec4(vertices[0].Position, 1.0f)) + glm::vec3(0.0f, 0.0f, 0.1f);
        //vertices.push_back(bunny->GetMeshes()[0].GetVertices()[hit.VertexIndex1]);
        //vertices[1].Position = glm::vec3(bunnyObj.Transform.ToMatrix() * glm::vec4(vertices[1].Position, 1.0f)) + glm::vec3(0.0f, 0.0f, 0.1f);
        //vertices.push_back(bunny->GetMeshes()[0].GetVertices()[hit.VertexIndex2]);
        //vertices[2].Position = glm::vec3(bunnyObj.Transform.ToMatrix() * glm::vec4(vertices[2].Position, 1.0f)) + glm::vec3(0.0f, 0.0f, 0.1f);
        //Mesh aabbMesh{ vertices, indices, MeshPrimitive::Triangle };
        //std::shared_ptr<Model> aabb = std::shared_ptr<Model>{ new Model { std::vector<Mesh>{ aabbMesh } } };
        //
        //SceneObject aabbObj{ aabb };
        //aabbObj.SetMeshMaterialLink(0, aabbMat);
        //Scene.SceneObjects.emplace_back(aabbObj);
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
        std::weak_ptr<Model> gizmo = AssetSystem::GetInstance()->CreateAndStoreModel("Gizmo");
        gizmo.lock()->Meshes.emplace_back(gizmosMesh);

        // shaders and materials
        std::weak_ptr<ShaderPipeline> unlit = AssetSystem::GetInstance()->LoadAndStoreShaderPipeline("GaladHen/Shaders/ShadingModels/Unlit/Unlit.vert", "", "", "", "GaladHen/Shaders/Materials/VertexUnlitColor.frag", "", "VertexUnlitColor");
        std::shared_ptr<Material> aabbMat{ new Material{ unlit } };
        aabbMat->Vec4Data.emplace("Diffuse", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        std::shared_ptr<Material> gizmosMat{ new Material{ unlit } };

        SceneObject gizmoObj{ gizmo };
        gizmoObj.SetMeshMaterialLink(0, gizmosMat);

        Scene.SceneObjects.emplace_back(gizmoObj);
    }

	void Editor::Run()
	{
        TimeSystem::GetInstance()->Start();

        while (!InputSystem::GetInstance()->IsCloseWindowRequested())
        {
            TimeSystem::GetInstance()->NewFrame();
            InputSystem::GetInstance()->UpdateMousePosition();

            glm::vec3 cameraLinMov = glm::vec3(0.0f);
            if (InputSystem::GetInstance()->IsKeyboardKeyPressed(KeyboardKey::W))
            {
                cameraLinMov.x += 1.0f;
            }
            if (InputSystem::GetInstance()->IsKeyboardKeyPressed(KeyboardKey::S))
            {
                cameraLinMov.x -= 1.0f;
            }
            if (InputSystem::GetInstance()->IsKeyboardKeyPressed(KeyboardKey::D))
            {
                cameraLinMov.z += 1.0f;
            }
            if (InputSystem::GetInstance()->IsKeyboardKeyPressed(KeyboardKey::A))
            {
                cameraLinMov.z -= 1.0f;
            }
            if (InputSystem::GetInstance()->IsKeyboardKeyPressed(KeyboardKey::E))
            {
                cameraLinMov.y += 1.0f;
            }
            if (InputSystem::GetInstance()->IsKeyboardKeyPressed(KeyboardKey::Q))
            {
                cameraLinMov.y -= 1.0f;
            }
            cameraLinMov *= TimeSystem::GetInstance()->GetDeltaTime() * 0.1f;
            glm::vec3 cameraPos = Scene.MainCamera.Transform.GetPosition();
            cameraPos += Scene.MainCamera.Transform.GetFront() * cameraLinMov.x;
            cameraPos += Scene.MainCamera.Transform.GetRight() * cameraLinMov.z;
            cameraPos += Scene.MainCamera.Transform.GetUp() * cameraLinMov.y;
            Scene.MainCamera.Transform.SetPosition(cameraPos);

            glm::vec2 cameraAngMov = glm::vec2(0.0f);
            if (InputSystem::GetInstance()->IsMouseKeyPressed(MouseKey::Right))
            {
                cameraAngMov = -InputSystem::GetInstance()->GetDeltaMousePosition();
            }
            cameraAngMov *= TimeSystem::GetInstance()->GetDeltaTime() * 5.0f;
            Scene.MainCamera.Transform.RotateYaw(cameraAngMov.x);
            Scene.MainCamera.Transform.RotatePitch(cameraAngMov.y);

            RenderingSystem::GetInstance()->Draw(Scene);
            RenderingSystem::GetInstance()->DrawUI();

            InputSystem::GetInstance()->InvokePendingCallbacks();
        }

        RenderingSystem::GetInstance()->CloseMainWindow();
	}
}
