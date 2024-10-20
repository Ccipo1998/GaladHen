
#include "MainPage.h"

namespace GaladHen
{
	UIMainPage::UIMainPage(const char* pageTitle, Window* ownerWindow)
		: UIPage(pageTitle, ownerWindow)
		, NavBar("NavBar", this)
		, AssetsView("Assets View", this)
		{}

	void UIMainPage::BuildPage()
	{
		UIPage::BuildPage();

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

		//ImGui::BeginDocked()

		ImGui::Begin("Test");
		


		ImGui::End();

		/*NavBar.BuildWidget();
		AssetsView.BuildWidget();*/
	}
}
