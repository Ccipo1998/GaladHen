
#include "AssetsViewWidget.h"
#include <Editor/UI/Widgets/NavBarWidget.h>

#include <imgui/imgui.h>

namespace GaladHen
{
	UIAssetsViewWidget::UIAssetsViewWidget(const char* widgetName)
		: UIWidget(widgetName)
	{}

	void UIAssetsViewWidget::Build()
	{	
		if (ImGui::Begin(WidgetName.data()))
		{
			//static bool dock_builder = true;
			//if (dock_builder)
			//{
			//	dock_builder = false;

			//	// init dockspace
			//	ImGuiID dockspace_id = ImGui::GetID(WidgetName.data());
			//	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

			//	// clear previous layout
			//	ImGui::DockBuilderRemoveNode(dockspace_id);

			//	// create new docking node
			//	ImGui::DockBuilderAddNode(dockspace_id);

			//	ImGui::DockBuilderFinish(dockspace_id);
			//}

			ImGui::End();
		}
	}
}
