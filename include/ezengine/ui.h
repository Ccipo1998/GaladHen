
#pragma once

// imgui
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <ezengine/material.h>

namespace UI
{
    // shading mode
    extern const char* ShadingModesNames[];
    extern const GLuint ShadingModesValues[];
    extern const unsigned int ShadingModesNumber;
    extern unsigned int ShadingModeSelected;

    // shading type
    extern const char* ShadingTypesNames[];
    extern const GLuint ShadingTypesValues[];
    extern const unsigned int ShadingTypesNumber;
    extern unsigned int ShadingTypeSelected;

    /*
    @brief
    Initialization of ImGui context
    @param window: target window
    @param glsl_version: glsl version to use to init imgui for opengl
    */
    void InitImGui(GLFWwindow* window, const char* glsl_version);
    
    /*
    @brief
    Create the new frame of the imgui UI
    */
    void Update(Material* mat);

    /*
    @brief
    Clear imgui rendered data
    */
    void Clear();

    /*
    @brief
    Destroy imgui context and free resources
    */
    void DestroyAndFree();
}
