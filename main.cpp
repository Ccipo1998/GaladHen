#include <iostream>

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

// glfw
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <ezengine/shader.h>
#include <ezengine/model.h>
#include <ezengine/camera.h>
#include <ezengine/light.h>
#include <ezengine/material.h>
#include <ezengine/ui.h>
#include <ezengine/input.h>
#include <ezengine/scene.h>
#include <ezengine/gameobject.h>
#include <ezengine/pbrmaterial.h>
#include <ezengine/textureimage.h>
#include <ezengine/texture.h>

// glm
#include <glm.hpp>
#include <gtx/transform.hpp> // for lookat() and perspective()
#include <gtc/type_ptr.hpp> // for value_ptr()

#define BUFFER_OFFSET(offset) (void*)(offset) // MACRO

// paths of shaders
#define PBR_VERTEX_SHADER_PATH "shaders/pbr/Pbr.vert"
#define PBR_FRAGMENT_SHADER_PATH "shaders/pbr/Pbr.frag"
#define PHONG_VERTEX_SHADER_PATH "shaders/phong/Phong.vert"
#define PHONG_FRAGMENT_SHADER_PATH "shaders/phong/Phong.frag"

// statics data

// current scene
Scene* CurrentScene;
// shaders
Shader* pbrShader;
Shader* phongShader;
// models
std::vector<Model*> CurrentModels;
// materials
std::vector<Material*> CurrentMaterials;

GLFWwindow* InitContext(int width, int height, const char* name);

int main()
{
    GLFWwindow* window = InitContext(1280, 720, "EzEngine");
    glfwMakeContextCurrent(window);

    // callbacks
    Input::LinkInputCallbacks(window);

    glClearColor(.1f, .1f, .1f, .0f);
    // we enable Z test
    glEnable(GL_DEPTH_TEST);

    // create scene
    CurrentScene = new Scene{};

    // init ui
    UI::InitImGui(window, "#version 460 core");
    //glEnable(GL_CULL_FACE);
    // load shaders
    pbrShader = new Shader{};
    pbrShader->LoadVertexFragmentShaders(PBR_VERTEX_SHADER_PATH, PBR_FRAGMENT_SHADER_PATH);
    // texture test
    TextureImage* texImgAlbedo = new TextureImage;
    texImgAlbedo->LoadTexture("textures/StuccoRoughCast001_COL_2K_METALNESS.png");
    texImgAlbedo->SendTextureDataToGPU(GL_TEXTURE0);
    Texture* texAlbedo = new Texture{texImgAlbedo};
    texAlbedo->SetUniformSamplerForShader("DiffuseColor", pbrShader);
    TextureImage* texImgNormal = new TextureImage;
    texImgNormal->LoadTexture("textures/StuccoRoughCast001_NRM_2K_METALNESS.png");
    texImgNormal->SendTextureDataToGPU(GL_TEXTURE1);
    Texture* texNormal = new Texture{texImgNormal};
    texNormal->SetUniformSamplerForShader("NormalMap", pbrShader);

    // game object
    CurrentModels.push_back(new Model("prefabs/cube.glb"));
    Model* model = CurrentModels[0];
    CurrentMaterials.push_back(new PBRMaterial{pbrShader});
    PBRMaterial* mat = (PBRMaterial*)(CurrentMaterials[0]);
    model->Meshes[0].MeshMaterial = mat;
    //PBRMaterial mat{};
    CurrentScene->GameObjects.push_back(new GameObject{});
    GameObject* object = CurrentScene->GameObjects[0];
    object->Model = model;

    // lights
    // CurrentScene->PointLights.push_back(new PointLight(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, glm::vec3(.0f, .0f, 5.0f)));
    // PointLight* pLight1 = CurrentScene->PointLights[0];
    // CurrentScene->PointLights.push_back(new PointLight(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, glm::vec3(.0f, .0f, -5.0f)));
    // PointLight* pLight2 = CurrentScene->PointLights[1];
    CurrentScene->DirectionalLights.push_back(new DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    DirectionalLight* dirLight = CurrentScene->DirectionalLights[0];

    // send light data to shader
    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    unsigned int pointLightsNumber = (unsigned int)CurrentScene->PointLights.size();
    GLsizeiptr pointLightsSize = sizeof(GLuint) + pointLightsNumber * PointLight::GetPointLightSizeForShader();
    glBufferData(GL_SHADER_STORAGE_BUFFER, pointLightsSize, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLuint), &pointLightsNumber);
    for (unsigned int i = 0; i < pointLightsNumber; ++i)
    {
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint) + PointLight::GetPointLightSizeForShader() * i, PointLight::GetPointLightSizeForShader(), CurrentScene->PointLights[i]->GetLightDataForShader());
    }
    GLuint ssbo1;
    glGenBuffers(1, &ssbo1);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo1);
    unsigned int directionalLightsNumber = (unsigned int)CurrentScene->DirectionalLights.size();
    GLsizeiptr directionalLightsSize = sizeof(GLuint) + directionalLightsNumber * DirectionalLight::GetDirLightSizeForShader();
    glBufferData(GL_SHADER_STORAGE_BUFFER, directionalLightsSize, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLuint), &directionalLightsNumber);
    for (unsigned int i = 0; i < directionalLightsNumber; ++i)
    {
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint) + DirectionalLight::GetDirLightSizeForShader() * i, DirectionalLight::GetDirLightSizeForShader(), CurrentScene->DirectionalLights[i]->GetLightDataForShader());
    }
    
    GLfloat lastTime = .0f;
    GLfloat deltaTime = .0f;
    while (!glfwWindowShouldClose(window))
    {
        // we "clear" the frame and z buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // time counter
        GLfloat currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // input update
        Input::Update();
        // ui update
        UI::Update(mat);

        // camera movements
        CurrentScene->MainCamera->ApplyMovements(Input::GetKeys(), Input::GetMouseKeys(), Input::GetDeltaMouseX(), Input::GetDeltaMouseY(), deltaTime);

        // send camera data to shaders -> may vary between frames
        glProgramUniformMatrix4fv(pbrShader->GetShaderProgram(), glGetUniformLocation(pbrShader->GetShaderProgram(), "NormalMatrix"), 1, GL_FALSE, value_ptr(glm::transpose(glm::inverse(CurrentScene->MainCamera->GetViewMatrix()))));
        glProgramUniformMatrix4fv(pbrShader->GetShaderProgram(), glGetUniformLocation(pbrShader->GetShaderProgram(), "ProjectionMatrix"), 1, GL_FALSE, value_ptr(CurrentScene->MainCamera->GetProjectionMatrix()));
        glProgramUniformMatrix4fv(pbrShader->GetShaderProgram(), glGetUniformLocation(pbrShader->GetShaderProgram(), "ViewMatrix"), 1, GL_FALSE, value_ptr(CurrentScene->MainCamera->GetViewMatrix()));

        // select shading subroutines

        GLuint subroutinesIndices[2];
        // IMPORTANT: subroutine uniform location (array index) <-> subroutine function index (value)
        subroutinesIndices[0] = UI::ShadingModesValues[UI::ShadingModeSelected];
        subroutinesIndices[1] = UI::ShadingTypesValues[UI::ShadingTypeSelected];
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, subroutinesIndices);

        /*
        glBindVertexArray(cube.VAO);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, cube.Vertices.size());
        */
        CurrentScene->Draw();

        // ui clear
        UI::Clear();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ui delete
    UI::DestroyAndFree();

    // frees
    pbrShader->Delete();
    for (auto& ptr : CurrentModels)
    {
        delete ptr;
    }
    for (auto& ptr : CurrentMaterials)
    {
        delete ptr;
    }
    // delete texAlbedo;
    // delete texImgAlbedo;

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

/*
@brief
Initialization of the OpenGL context (glfw & gl3w) and of the window
@param width: window width
@param height: window height
@param name: window name
*/
GLFWwindow* InitContext(int width, int height, const char* name)
{
    if (!glfwInit())
        throw std::runtime_error("Error: GLFW failed to initialize the context");

    // setting the minimum required version of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    // core profile is a subset of OpenGL features (without the backward-compatible features)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // for MacOS:
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    if (!gl3wInit())
        throw std::runtime_error("Error: GL3W failed to initialize the context");

    GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    if (window == nullptr)
        throw std::runtime_error("Error: GLFW failed to create the window");

    return window;
}
