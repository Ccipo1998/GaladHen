
#include "ViewportWidget.h"

#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <Editor/Editor.h>
#include <Renderer/RenderBuffer.h>

namespace GaladHen
{
	UIViewportWidget::UIViewportWidget(const char* widgetName, UIPage* ownerPage)
		: UIWidget(widgetName, ownerPage)
	{}

	void UIViewportWidget::BuildWidget()
	{
		// Parent dockspace
		ImGuiID dockspace_id = ImGui::GetID("Root");

		static bool first_time = true;
		if (first_time)
		{
			first_time = false;

			auto dock_id_center = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 1.0f, nullptr, &dockspace_id);

			ImGui::DockBuilderDockWindow(WidgetName.data(), dock_id_center);

			ImGui::DockBuilderFinish(dockspace_id);
		}

		ImGui::Begin(WidgetName.data());
		ImGui::GetWindowDrawList()->AddImage(Editor::GetEditorRenderer().GetRenderBuffer().GetRenderBufferColorID(), ImGui::GetWindowContentRegionMin(), ImGui::GetWindowContentRegionMax(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
	}
}
