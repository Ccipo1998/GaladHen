
#include <ezengine/ui.h>

namespace UI
{
    const char* ShadingModesNames[] = { "Smooth", "Flat" };
    const GLuint ShadingModesValues[] = { 0, 1 };
    const unsigned int ShadingModesNumber = 2;
    unsigned int ShadingModeSelected = 0;

    const char* ShadingTypesNames[] = { "Phong Diffuse Reflection", "Phong Shading Model" };
    const GLuint ShadingTypesValues[] = { 2, 3 };
    const unsigned int ShadingTypesNumber = 2;
    unsigned int ShadingTypeSelected = 0;
}

void UI::InitImGui(GLFWwindow* window, const char* glsl_version)
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

void UI::Update(Material* mat)
{
    // ImGui

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Shading parameters");
    if (ImGui::BeginCombo("Shading Mode", UI::ShadingModesNames[UI::ShadingModeSelected]))
    {
        for (unsigned int i = 0; i < UI::ShadingModesNumber; ++i)
        {
            bool selected = UI::ShadingModeSelected == i;
            if (ImGui::Selectable(ShadingModesNames[i], selected))
            {
                UI::ShadingModeSelected = i;
            }

            if (selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("Shading Type", UI::ShadingTypesNames[UI::ShadingTypeSelected]))
    {
        for (unsigned int i = 0; i < UI::ShadingTypesNumber; ++i)
        {
            bool selected = UI::ShadingTypeSelected == i;
            if (ImGui::Selectable(ShadingTypesNames[i], selected))
            {
                UI::ShadingTypeSelected = i;
            }

            if (selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }
    ImGui::End();

    ImGui::Begin("Material parameters");
    ImGui::ColorPicker3("Diffuse Color", &mat->DiffuseColor.x);
    //ImGui::SliderFloat("Specular", &mat.Specular, .0f, 1.f);
    //ImGui::SliderFloat("Kd", &mat.Kd, .0f, 1.0f);
    ImGui::SliderFloat("Metallic", &mat->Metallic, .0f, 1.0f);
    ImGui::SliderFloat("Roughness", &mat->Roughness, .0f, 1.f);
    ImGui::End();

    ImGui::Render();
}

void UI::Clear()
{
    // gui cleaning
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::DestroyAndFree()
{
    // gui delete
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
