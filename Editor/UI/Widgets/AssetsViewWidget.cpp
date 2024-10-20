
#include "AssetsViewWidget.h"
#include <Editor/UI/Page.h>
#include <Renderer/Window.h>
#include <Editor/UI/Widgets/NavBarWidget.h>

#include <imgui/imgui.h>

namespace GaladHen
{
	UIAssetsViewWidget::UIAssetsViewWidget(const char* widgetName, UIPage* ownerPage)
		: UIWidget(widgetName, ownerPage)
	{}

	void UIAssetsViewWidget::BuildWidget()
	{	
		/*glm::uvec2 pos = OwnerPage->GetOwningWindow()->GetWindowPosition();
		glm::uvec2 size = OwnerPage->GetOwningWindow()->GetWindowSize();*/

		if (ImGui::Begin("Assets View"))
		{
			/*ImGui::SetWindowPos(ImVec2{ (float)pos.x, (float)pos.y });
			ImGui::SetWindowSize(ImVec2{ (float)size.x * WidthScale, (float)size.y * HeightScale });*/

			ImGui::End();
		}
	}
}
