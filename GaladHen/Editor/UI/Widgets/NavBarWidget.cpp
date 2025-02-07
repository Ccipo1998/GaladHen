
#include "NavBarWidget.h"

#include <imgui/imgui.h>
#include <glm/glm.hpp>

namespace GaladHen
{
	UINavBarWidget::UINavBarWidget(const char* widgetName)
		: UIWidget(widgetName)
		{}

	void UINavBarWidget::Build()
	{
		if (ImGui::BeginMenuBar())
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
			ImGui::EndMenuBar();
		}
	}
}
