
#include "NavBarWidget.h"

#include <imgui/imgui.h>

namespace GaladHen
{
	UINavBarWidget::UINavBarWidget(const char* widgetName)
		: UIWidget(widgetName)
		{}

	void UINavBarWidget::BuildWidget()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				bool loadModelSelected = false;
				if (ImGui::MenuItem("Load Model", nullptr, &loadModelSelected))
				{
					if (loadModelSelected)
					{

					}
				}

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}
