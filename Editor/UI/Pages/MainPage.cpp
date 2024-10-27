
#include "MainPage.h"

namespace GaladHen
{
	UIMainPage::UIMainPage(const char* pageTitle, Window* ownerWindow)
		: UIPage(pageTitle, ownerWindow)
		, NavBar("NavBar", this)
		, AssetsView("Assets View", this)
		, Viewport("Viewport", this)
		{}

	void UIMainPage::BuildPage()
	{
		UIPage::BuildPage();

		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
		static ImGuiWindowFlags root_window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::Begin("Root", nullptr, root_window_flags);

		// Dockspace
		ImGuiID dockspace_id = ImGui::GetID("Root");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		static bool first_time = true;
		if (first_time)
		{
			first_time = false;

			ImGui::DockBuilderRemoveNode(dockspace_id);
			ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);
			ImGui::DockBuilderFinish(dockspace_id);
		}

		Viewport.BuildWidget();

		ImGui::End();
	}
}
