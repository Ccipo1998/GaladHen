
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
#include <Renderer/Entities/ShaderPipeline.h>

#include <glm/glm.hpp>
#include <imgui/imgui.h>

#include <string>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <utils/log.h>
#include <utils/FileLoader.h>

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
        AssetsManager::LoadAndStoreShaderPipeline("GaladHen/Shaders/pbr/Pbr.vert", "", "", "", "GaladHen/Shaders/pbr/Pbr.frag", "", "PBR");

        AddDefaultGizmosToScene();

        //if (!glfwInit())
        //{
        //    Log::Error("RendererGL", "GLFW failed to initialize the context");

        //    return;
        //}

        //// setting the minimum required version of OpenGL
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLSL_VERSION_MAJOR);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLSL_VERSION_MINOR);
        //// core profile is a subset of OpenGL features (without the backward-compatible features)
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //// for MacOS:
        ////glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        //GLFWwindow* WinGL = glfwCreateWindow(1920, 1080, "Test", nullptr, nullptr);
        //if (WinGL == nullptr)
        //{
        //    Log::Error("WindowGL", "GLFW failed to create the window");

        //    return;
        //}

        //glfwMakeContextCurrent(WinGL);

        //if (gl3wInit() != GL3W_OK)
        //{
        //    Log::Error("RendererGL", "Error: GL3W failed to initialize the context");

        //    return;
        //}

        //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //glEnable(GL_DEPTH_TEST);

        //std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5 };
        //std::vector<MeshVertexData> vertices;
        //MeshVertexData v{};
        //v.Position = glm::vec3(0.0f);
        //v.Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        //vertices.push_back(v);
        //v.Position = glm::vec3(1.0f, 0.0f, 0.0f);
        //vertices.push_back(v);
        //v.Color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        //v.Position = glm::vec3(0.0f);
        //vertices.push_back(v);
        //v.Position = glm::vec3(0.0f, 1.0f, 0.0f);
        //vertices.push_back(v);
        //v.Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        //v.Position = glm::vec3(0.0f);
        //vertices.push_back(v);
        //v.Position = glm::vec3(0.0f, 0.0f, 1.0f);
        //vertices.push_back(v);
        //Mesh gizmosMesh{ vertices, indices, MeshPrimitive::Line };

        //Camera camera{};
        //camera.Transform.SetPosition(glm::vec3(.0f, 1.0f, 4.0f));
        //camera.Transform.RotateYaw(90.0f);

        //GLuint VAO, VBO, EBO;
        //glGenVertexArrays(1, &VAO);
        //glGenBuffers(1, &VBO);
        //glGenBuffers(1, &EBO);

        //glBindVertexArray(VAO);
        //// we copy data in the VBO - we must set the data dimension, and the pointer to the structure cointaining the data
        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, gizmosMesh.GetVertices().size() * sizeof(MeshVertexData), &gizmosMesh.GetVertices()[0], GL_STATIC_DRAW);
        //// we copy data in the EBO - we must set the data dimension, and the pointer to the structure cointaining the data
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, gizmosMesh.GetIndices().size() * sizeof(GLuint), &gizmosMesh.GetIndices()[0], GL_STATIC_DRAW);

        //// we set in the VAO the pointers to the different vertex attributes (with the relative offsets inside the data structure)
        //// vertex positions
        //// these will be the positions to use in the layout qualifiers in the shaders ("layout (location = ...)"")
        //glEnableVertexAttribArray(0);
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)0);
        //// Normals
        //glEnableVertexAttribArray(1);
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)offsetof(MeshVertexData, Normal));
        //// Texture Coordinates
        //glEnableVertexAttribArray(2);
        //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)offsetof(MeshVertexData, UV));
        //// Tangent
        //glEnableVertexAttribArray(3);
        //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)offsetof(MeshVertexData, Tangent));
        //// Bitangent
        //glEnableVertexAttribArray(4);
        //glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)offsetof(MeshVertexData, Bitangent));
        //// Vertex color
        //glEnableVertexAttribArray(5);
        //glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)offsetof(MeshVertexData, Color));

        //glBindVertexArray(0);

        //std::shared_ptr<ShaderPipeline> unlit = AssetsManager::LoadAndStoreShaderPipeline("../Shaders/Unlit/Unlit.vert", "", "", "", "../Shaders/Unlit/Unlit.frag", "", "Unlit");
        //auto vPath = unlit->GetVertexShaderPath();
        //auto fPath = unlit->GetFragmentShaderPath();
        //auto vStringCode = FileLoader::ReadTextFile(vPath);
        //const char* vCode = vStringCode.c_str();
        //auto fStringCode = FileLoader::ReadTextFile(fPath);
        //const char* fCode = fStringCode.c_str();

        //GLuint program = glCreateProgram();

        //// compile the shaders
        //GLuint vShader, fShader;

        //vShader = glCreateShader(GL_VERTEX_SHADER);
        //glShaderSource(vShader, 1, &vCode, NULL);
        //glCompileShader(vShader);

        //glAttachShader(program, vShader);

        //fShader = glCreateShader(GL_FRAGMENT_SHADER);
        //glShaderSource(fShader, 1, &fCode, NULL);
        //glCompileShader(fShader);

        //glAttachShader(program, fShader);

        //// link
        //glLinkProgram(program);

        //glDeleteShader(vShader);
        //glDeleteShader(fShader);

        //GLuint cameraBuffer, transformBuffer;
        //glCreateBuffers(1, &cameraBuffer);
        //glBindBuffer(GL_UNIFORM_BUFFER, cameraBuffer);
        //glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2 + sizeof(glm::vec3), nullptr, GL_STATIC_READ);
        //glm::mat4 viewMatrix = camera.GetViewMatrix();
        //glm::mat4 projMatrix = camera.GetProjectionMatrix();
        //glm::vec3 cameraPos = camera.Transform.GetPosition();
        //glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &viewMatrix);
        //glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projMatrix);
        //glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::vec3), &cameraPos);
        //glBindBuffer(GL_UNIFORM_BUFFER, 0);

        //glCreateBuffers(1, &transformBuffer);
        //glBindBuffer(GL_UNIFORM_BUFFER, transformBuffer);
        //glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, nullptr, GL_STATIC_READ);
        //Transform objTransform = Transform{};
        //glm::mat4 modelMatrix = objTransform.ToMatrix();
        //glm::mat4 normalMatrix = glm::inverse(glm::transpose(modelMatrix));
        //glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &modelMatrix);
        //glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &normalMatrix);
        //glBindBuffer(GL_UNIFORM_BUFFER, 0);

        //while (!glfwWindowShouldClose(WinGL))
        //{
        //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //    /*std::string subroutine = "VertexColorConstant";
        //    GLuint subLocation = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, subroutine.data());
        //    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &subLocation);*/

        //    glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraBuffer);
        //    glBindBufferBase(GL_UNIFORM_BUFFER, 1, transformBuffer);

        //    glUseProgram(program);
        //    glBindVertexArray(VAO);
        //    glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
        //    glBindVertexArray(0);


        //    glfwSwapBuffers(WinGL);
        //    glfwPollEvents();
        //}
	}

    void Editor::AddDefaultBunnyToScene()
    {
        // load textures
        std::shared_ptr<Texture> texAlbedo = AssetsManager::LoadAndStoreTexture(
            "GaladHen/Assets/Textures/StuccoRoughCast001_COL_2K_METALNESS.png",
            "StuccoAlbedo",
            TextureFormat::SRGB8);
        texAlbedo->SetNumberOfMipMaps(4);
        std::shared_ptr<Texture> texNormal = AssetsManager::LoadAndStoreTexture(
            "GaladHen/Assets/Textures/StuccoRoughCast001_NRM_2K_METALNESS.png",
            "StuccoNormal",
            TextureFormat::RGB8);
        texNormal->SetNumberOfMipMaps(4);
        std::shared_ptr<Texture> texRoughness = AssetsManager::LoadAndStoreTexture(
            "GaladHen/Assets/Textures/StuccoRoughCast001_ROUGHNESS_2K_METALNESS.png",
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
        std::shared_ptr<Model> bunny = AssetsManager::LoadAndStoreModel("GaladHen/Assets/Models/bunny.glb", "Bunny");
        std::shared_ptr<Model> plane = AssetsManager::LoadAndStoreModel("GaladHen/Assets/Models/plane.glb", "Plane");

        // bvh
        bunny->GetMeshes()[0].BVH.BuildBVH(bunny->GetMeshes()[0], AABBSplitMethod::PlaneCandidates);
        bunny->BVH.BuildBVH(*bunny, AABBSplitMethod::PlaneCandidates);

        // shaders and materials
        std::shared_ptr<ShaderPipeline> unlit = AssetsManager::LoadAndStoreShaderPipeline("GaladHen/Shaders/Unlit/Unlit.vert", "", "", "", "GaladHen/Shaders/Unlit/Unlit.frag", "", "Unlit");
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
        std::shared_ptr<ShaderPipeline> unlit = AssetsManager::LoadAndStoreShaderPipeline("GaladHen/Shaders/ShadingModels/Unlit/Unlit.vert", "", "", "", "GaladHen/Shaders/Materials/VertexUnlitColor/VertexUnlitColor.frag", "", "Unlit");
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

            EditorUIPage->NewFrame();
            EditorUIPage->BuildPage();
            EditorUIPage->Draw();

            Window.EndFrame();
        }

        Window.CloseWindow();


	}
}
