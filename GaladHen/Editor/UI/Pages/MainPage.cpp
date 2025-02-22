
#include "MainPage.h"

#include <imgui/imgui.h>

#include <Editor/UI/Widgets/NavBarWidget.h>
#include <Editor/UI/Widgets/AssetsViewWidget.h>
#include <Editor/UI/Widgets/ViewportWidget.h>
#include <Editor/UI/Widgets/InspectorWidget.h>

#include <glm/glm.hpp>

namespace GaladHen
{
	UIMainPage::UIMainPage(const char* pageName)
		: UIPage(pageName)
	{
		// Populate widgets for main page
		Widgets.emplace_back(new UINavBarWidget{ "NavBar" });
		Widgets.emplace_back(new UIViewportWidget{ "Viewport" });
		Widgets.emplace_back(new UIInspectorWidget{ "Inspector" });
	}

	void UIMainPage::Build()
	{
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		static ImGuiWindowFlags root_window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin(PageName.data(), nullptr, root_window_flags);

		ImGui::PopStyleVar();

        ImGuiID dockspace_id = ImGui::GetID("RootDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		BuildWidgets();

		ImGui::End();
	}
}
