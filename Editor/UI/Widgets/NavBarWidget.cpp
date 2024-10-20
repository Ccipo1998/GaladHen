
#include "NavBarWidget.h"
#include <Editor/UI/Page.h>
#include <Renderer/Window.h>

#include <imgui/imgui.h>
#include <glm/glm.hpp>

namespace GaladHen
{
	unsigned int UINavBarWidget::Height = 10;

	UINavBarWidget::UINavBarWidget(const char* widgetName, UIPage* ownerPage)
		: UIWidget(widgetName, ownerPage)
		{}

	void UINavBarWidget::BuildWidget()
	{
		if (ImGui::BeginMainMenuBar())
		{
			ImVec2 size = ImGui::GetWindowSize();
			Height = size.y;

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

	unsigned int UINavBarWidget::GetNavBarHeight()
	{
		return Height;
	}
}
