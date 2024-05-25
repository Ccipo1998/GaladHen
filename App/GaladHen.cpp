
#include <iostream>

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

// glfw
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <GaladHen/shader.h>
#include <GaladHen/model.h>
#include <GaladHen/camera.h>
#include <GaladHen/light.h>
#include <GaladHen/material.h>
#include <GaladHen/ui.h>
#include <GaladHen/input.h>
#include <GaladHen/scene.h>
#include <GaladHen/sceneobject.h>
#include <GaladHen/pbrmaterial.h>
#include <GaladHen/textureimage.h>
#include <GaladHen/texture.h>
#include <GaladHen/AssetsManager.h>

// glm
#include <glm.hpp>
#include <gtx/transform.hpp> // for lookat() and perspective()
#include <gtc/type_ptr.hpp> // for value_ptr()

#define BUFFER_OFFSET(offset) (void*)(offset) // MACRO

// paths of shaders
#define PBR_VERTEX_SHADER_PATH "Shaders/pbr/Pbr.vert"
#define PBR_FRAGMENT_SHADER_PATH "Shaders/pbr/Pbr.frag"
#define PHONG_VERTEX_SHADER_PATH "Shaders/phong/Phong.vert"
#define PHONG_FRAGMENT_SHADER_PATH "Shaders/phong/Phong.frag"

// statics data

// current scene
Scene* CurrentScene;
// shaders
Shader* pbrShader;
Shader* postProcShader;

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
    texImgAlbedo->SendTextureDataToGPU(GL_SRGB8, GL_RGB);
    AssetsManager::SaveTexImageWithName(texImgAlbedo, "StuccoColor");
    TextureImage* texImgNormal = new TextureImage;
    texImgNormal->LoadTexture("textures/StuccoRoughCast001_NRM_2K_METALNESS.png");
    texImgNormal->SendTextureDataToGPU(GL_RGB8, GL_RGB);
    AssetsManager::SaveTexImageWithName(texImgNormal, "StuccoNormal");
    TextureImage* texImgRoughness = new TextureImage;
    texImgRoughness->LoadTexture("textures/StuccoRoughCast001_ROUGHNESS_2K_METALNESS.png");
    texImgRoughness->SendTextureDataToGPU(GL_RGB, GL_RED);
    AssetsManager::SaveTexImageWithName(texImgRoughness, "StuccoRoughness");

    AssetsManager::SaveModelWithName(new Model("prefabs/bunny.glb"), "Bunny");
    CurrentScene->SceneObjects.push_back(new SceneObject{AssetsManager::GetModelByName("Bunny")});
    SceneObject* object = CurrentScene->SceneObjects[0];

    PBRMaterial* pbrMat = new PBRMaterial{pbrShader};
    pbrMat->MaterialShadingMode = ShadingMode::SmoothShading;
    pbrMat->DiffuseTexture = 
    pbrMat->DiffuseTexture->SetUniformSamplerForShader("DiffuseColor", pbrShader);
    pbrMat->NormalMap = 
    pbrMat->NormalMap->SetUniformSamplerForShader("NormalMap", pbrShader);
    pbrMat->RoughnessTexture = 
    pbrMat->RoughnessTexture->SetUniformSamplerForShader("RoughnessTexture", pbrShader);

    object->SetMaterial(0, pbrMat);

    // lights
    CurrentScene->PointLights.push_back(new PointLight(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, glm::vec3(0.0f, 0.0f, 5.0f)));
    PointLight* pLight1 = CurrentScene->PointLights[0];
    // CurrentScene->PointLights.push_back(new PointLight(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, glm::vec3(.0f, .0f, -5.0f)));
    // PointLight* pLight2 = CurrentScene->PointLights[1];
    // CurrentScene->DirectionalLights.push_back(new DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, glm::vec3(0.0f, 0.0f, -1.0f)));
    // DirectionalLight* dirLight = CurrentScene->DirectionalLights[0];

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
        UI::Update(pbrMat);

        // camera movements
        CurrentScene->MainCamera->ApplyMovements(Input::GetKeys(), Input::GetMouseKeys(), Input::GetDeltaMouseX(), Input::GetDeltaMouseY(), deltaTime);

        // send camera data to shaders -> may vary between frames
        glProgramUniformMatrix4fv(pbrShader->GetShaderProgram(), glGetUniformLocation(pbrShader->GetShaderProgram(), "NormalMatrix"), 1, GL_FALSE, value_ptr(glm::transpose(glm::inverse(CurrentScene->MainCamera->GetViewMatrix()))));
        glProgramUniformMatrix4fv(pbrShader->GetShaderProgram(), glGetUniformLocation(pbrShader->GetShaderProgram(), "ProjectionMatrix"), 1, GL_FALSE, value_ptr(CurrentScene->MainCamera->GetProjectionMatrix()));
        glProgramUniformMatrix4fv(pbrShader->GetShaderProgram(), glGetUniformLocation(pbrShader->GetShaderProgram(), "ViewMatrix"), 1, GL_FALSE, value_ptr(CurrentScene->MainCamera->GetViewMatrix()));
        glProgramUniform3f(pbrShader->GetShaderProgram(), glGetUniformLocation(pbrShader->GetShaderProgram(), "WCameraPosition"), CurrentScene->MainCamera->Transform.GetPosition().x, CurrentScene->MainCamera->Transform.GetPosition().y, CurrentScene->MainCamera->Transform.GetPosition().z);

        // select shading subroutines

        // GLuint subroutinesIndices[2];
        // // IMPORTANT: subroutine uniform location (array index) <-> subroutine function index (value)
        // subroutinesIndices[0] = UI::ShadingModesValues[UI::ShadingModeSelected];
        // subroutinesIndices[1] = UI::ShadingTypesValues[UI::ShadingTypeSelected];
        // glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, subroutinesIndices);

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

