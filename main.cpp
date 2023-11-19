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

// glm
#include <glm.hpp>
#include <gtx/transform.hpp> // for lookat() and perspective()
#include <gtc/type_ptr.hpp> // for value_ptr()

#define BUFFER_OFFSET(offset) (void*)(offset) // MACRO

// current scene
Scene currentScene;

GLFWwindow* initContext(int width, int height, const char* name);

int main()
{
    GLFWwindow* window = initContext(1200, 800, "EzEngine");
    glfwMakeContextCurrent(window);

    // callbacks
    Input::LinkInputCallbacks(window);

    glClearColor(.1f, .1f, .1f, .0f);
    // we enable Z test
    glEnable(GL_DEPTH_TEST);
    
    // init ui
    UI::InitImGui(window, "#version 450 core");

    // game object
    Model* model = new Model("prefabs/bunny.obj");
    Material* mat = new Material();
    mat->Ka = vec3(.1f, .1f, .1f);
    mat->Kd = vec3(.6f, .6f, .6f);
    mat->Ks = vec3(.8f, .8f, .8f);
    mat->SpecularFalloff = 100.0f;
    mat->DiffuseColor = vec3(1.0f, .0f, .0f);
    //mat.Specular = .0f;
    mat->Metallic = 1.0f;
    mat->Roughness = .0f;
    GameObject* object = new GameObject();
    object->Model = model;
    object->Material = mat;
    currentScene.GameObjects.push_back(object);

    // lights
    PointLight* pLight1 = new PointLight(vec3(1.0f, 1.0f, 1.0f), 1.0f, vec3(.0f, .0f, 5.0f));
    PointLight* pLight2 = new PointLight(vec3(1.0f, 1.0f, 1.0f), 1.0f, vec3(.0f, .0f, -5.0f));
    currentScene.PointLights.push_back(pLight1);
    currentScene.PointLights.push_back(pLight2);
    DirectionalLight* dirLight = new DirectionalLight(vec3(1.0f, 1.0f, 1.0f), 1.0f, vec3(1.0f, .0f, .0f));
    currentScene.DirectionalLights.push_back(dirLight);

    Shader shader = Shader("shaders/rendering/Rendering.vert", "shaders/rendering/Rendering.frag");
    shader.use();

    // send light data to shader
    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    unsigned int pointLightsNumber = (unsigned int)currentScene.PointLights.size();
    GLsizeiptr pointLightsSize = sizeof(GLuint) + pointLightsNumber * sizeof(PointLight);
    glBufferData(GL_SHADER_STORAGE_BUFFER, pointLightsSize, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLuint), &pointLightsNumber);
    for (unsigned int i = 0; i < pointLightsNumber; ++i)
    {
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint) + sizeof(PointLight) * i, sizeof(PointLight), currentScene.PointLights[i]);
    }
    GLuint ssbo1;
    glGenBuffers(1, &ssbo1);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo1);
    unsigned int directionalLightsNumber = (unsigned int)currentScene.DirectionalLights.size();
    GLsizeiptr directionalLightsSize = sizeof(GLuint) + directionalLightsNumber * sizeof(DirectionalLight);
    glBufferData(GL_SHADER_STORAGE_BUFFER, directionalLightsSize, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLuint), &directionalLightsNumber);
    for (unsigned int i = 0; i < directionalLightsNumber; ++i)
    {
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint) + sizeof(DirectionalLight) * i, sizeof(DirectionalLight), currentScene.DirectionalLights[i]);
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
        currentScene.MainCamera.applyMovements(Input::GetKeys(), Input::GetMouseKeys(), Input::GetDeltaMouseX(), Input::GetDeltaMouseY(), deltaTime);

        // send camera data to shaders -> may vary between frames
        glProgramUniformMatrix4fv(shader.Program, glGetUniformLocation(shader.Program, "NormalMatrix"), 1, GL_FALSE, value_ptr(glm::transpose(glm::inverse(currentScene.MainCamera.getViewMatrix()))));
        glProgramUniformMatrix4fv(shader.Program, glGetUniformLocation(shader.Program, "ProjectionMatrix"), 1, GL_FALSE, value_ptr(currentScene.MainCamera.getProjectionMatrix()));
        glProgramUniformMatrix4fv(shader.Program, glGetUniformLocation(shader.Program, "ViewMatrix"), 1, GL_FALSE, value_ptr(currentScene.MainCamera.getViewMatrix()));
        // send material data to shader
        glProgramUniform3fv(shader.Program, glGetUniformLocation(shader.Program, "Ka"), 1, value_ptr(mat->Ka));
        glProgramUniform3fv(shader.Program, glGetUniformLocation(shader.Program, "Kd"), 1, value_ptr(mat->Kd));
        glProgramUniform3fv(shader.Program, glGetUniformLocation(shader.Program, "Ks"), 1, value_ptr(mat->Ks));
        glProgramUniform1f(shader.Program, glGetUniformLocation(shader.Program, "SpecularFalloff"), mat->SpecularFalloff);
        glProgramUniform3fv(shader.Program, glGetUniformLocation(shader.Program, "Diffuse"), 1, value_ptr(mat->DiffuseColor));
        //glProgramUniform1f(shader.Program, glGetUniformLocation(shader.Program, "Specular"), mat.Specular);
        glProgramUniform1f(shader.Program, glGetUniformLocation(shader.Program, "Metallic"), mat->Metallic);
        glProgramUniform1f(shader.Program, glGetUniformLocation(shader.Program, "Roughness"), mat->Roughness);

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
        currentScene.Draw();

        // ui clear
        UI::Clear();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ui delete
    UI::DestroyAndFree();

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
GLFWwindow* initContext(int width, int height, const char* name)
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
