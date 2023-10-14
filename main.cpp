#include <iostream>

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "include/utils/shader.h"
#include "include/utils/model.h"
#include "include/utils/camera.h"
#include "include/utils/light.h"
#include "include/utils/material.h"

#include "include/glm/glm.hpp"
#include "include/glm/gtx/transform.hpp" // for lookat() and perspective()
#include "include/glm/gtc/type_ptr.hpp" // for value_ptr()

// imgui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define BUFFER_OFFSET(offset) (void*)(offset) // MACRO

// array for mouse buttons
bool mouse_keys[12];

// main camera
Camera mainCamera = Camera(vec3(.0f, .0f, 15.0f), radians(45.0f), 600.0f/400.0f, .1f, 100.0f);

// we initialize an array of booleans for each keybord key
bool keys[1024];
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
GLFWwindow* initContext(int width, int height, const char* name);
void initImGui(GLFWwindow* window, const char* glsl_version);

using namespace std;
using namespace glm;

int main()
{
    GLFWwindow* window = initContext(1200, 800, "EzEngine");
    glfwMakeContextCurrent(window);

    // callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glClearColor(.0f, .0f, .0f, .0f);
    // we enable Z test
    glEnable(GL_DEPTH_TEST);
    
    initImGui(window, "#version 450 core");

    Model model = Model("prefabs/cube.obj");

    Shader shader = Shader("shaders/pbr/GGX.vert", "shaders/pbr/GGX.frag");
    
    //mainCamera = Camera(vec3(.0f, .0f, 15.0f), radians(45.0f), 600.0f/400.0f, .1f, 100.0f);
    mainCamera.setYaw(90.0f);
    //camera.lookAt(vec3(.0f, .0f, .0f), vec3(.0f, 1.0f, .0f));

    //mat4 viewMatrix = lookAt(vec3(.0f, .0f, -1.0f), vec3(.0f, .0f, 3.0f), vec3(.0f, 1.0f, .0f));
    //mat4 projectionMatrix = perspective(radians(170.0f), 600.0f/400.0f, .1f, 100.0f);
    
    PointLight pointLight = PointLight(vec3(1.0f, 1.0f, 1.0f), 1.0f, vec3(3.0f, .0f, 3.0f));

    Material mat;
    mat.DiffuseColor = vec3(1.0f, .0f, .0f);
    //mat.Specular = .0f;
    mat.Metallic = 1.0f;
    mat.Roughness = .0f;

    shader.use();

    // mouse positions for delta
    double mouseLastX = 0.f;
    double mouseLastY = 0.f;
    double mouseX = 0.f;
    double mouseY = 0.f;
    
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

        // mouse position update
        // position update
        mouseLastX = mouseX;
        mouseLastY = mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Material parameters");
        ImGui::ColorPicker3("Diffuse Color", &mat.DiffuseColor.x);
        //ImGui::SliderFloat("Specular", &mat.Specular, .0f, 1.f);
        ImGui::SliderFloat("Metallic", &mat.Metallic, .0f, 1.0f);
        ImGui::SliderFloat("Roughness", &mat.Roughness, .0f, 1.f);
        ImGui::End();

        ImGui::Render();

        // camera movements
        mainCamera.applyMovements(keys, mouse_keys, mouseX - mouseLastX, mouseY - mouseLastY, deltaTime);

        // send camera data to shaders
        glProgramUniformMatrix4fv(shader.Program, glGetUniformLocation(shader.Program, "projectionMatrix"), 1, GL_FALSE, value_ptr(mainCamera.getProjectionMatrix()));
        glProgramUniformMatrix4fv(shader.Program, glGetUniformLocation(shader.Program, "viewMatrix"), 1, GL_FALSE, value_ptr(mainCamera.getViewMatrix()));
        glProgramUniform3fv(shader.Program, glGetUniformLocation(shader.Program, "lightPos"), 1, value_ptr(pointLight.Position));
        glProgramUniform3fv(shader.Program, glGetUniformLocation(shader.Program, "DiffuseColor"), 1, value_ptr(mat.DiffuseColor));
        //glProgramUniform1f(shader.Program, glGetUniformLocation(shader.Program, "Specular"), mat.Specular);
        glProgramUniform1f(shader.Program, glGetUniformLocation(shader.Program, "Metallic"), mat.Metallic);
        glProgramUniform1f(shader.Program, glGetUniformLocation(shader.Program, "Roughness"), mat.Roughness);

        /*
        glBindVertexArray(cube.VAO);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, cube.Vertices.size());
        */
        model.draw();

        // gui cleaning
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // gui delete
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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
        throw runtime_error("Error: GLFW failed to initialize the context");

    // setting the minimum required version of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    // core profile is a subset of OpenGL features (without the backward-compatible features)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // for MacOS:
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    if (!gl3wInit())
        throw runtime_error("Error: GL3W failed to initialize the context");

    GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    if (window == nullptr)
        throw runtime_error("Error: GLFW failed to create the window");

    return window;
}

void initImGui(GLFWwindow* window, const char* glsl_version)
{
     // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

/*
@brief
Keys callbacks responses
@param window: pointer to the GLFWwindow created when the context was initialized
@param key: integer representing the activated key
@param action: integer representing the action on the key
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // we keep trace of the pressed keys
    // with this method, we can manage 2 keys pressed at the same time:
    // many I/O managers often consider only 1 key pressed at the time (the first pressed, until it is released)
    // using a boolean array, we can then check and manage all the keys pressed at the same time
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(action == GLFW_PRESS)
        mouse_keys[button] = true;
    else if(action == GLFW_RELEASE)
        mouse_keys[button] = false;
}

