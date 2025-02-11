
#include "ViewportWidget.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <Editor/Editor.h>
#include <Systems/RenderingSystem/Entities/RenderBuffer.h>
#include <Utils/Log.h>

namespace GaladHen
{
	UIViewportWidget::UIViewportWidget(const char* widgetName)
		: UIWidget(widgetName)
	{}

	void UIViewportWidget::Build()
	{	
		static bool first_execution = true;
		if (first_execution)
		{
			ImGuiID dockspace_id = ImGui::GetID("RootDockSpace");
			ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockspace_id);
			ImGui::DockBuilderAddNode(dockspace_id);
			ImGuiID left_id, right_id;
			ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.3f, &left_id, &right_id);
			ImGui::DockBuilderDockWindow(WidgetName.data(), left_id);

			first_execution = false;
		}

		ImGui::Begin(WidgetName.data());

		ImVec2 pos = ImGui::GetWindowPos();
		pos.x += 10.0f;
		pos.y += 10.0f;
		ImVec2 size = ImGui::GetWindowPos();
		size.x += ImGui::GetWindowSize().x - 10.0f;
		size.y += ImGui::GetWindowSize().y - 10.0f;

		std::weak_ptr<RenderBuffer> frontBuffer = RenderingSystem::GetInstance()->GetFrontRenderBuffer();
		if (std::shared_ptr<RenderBuffer> shFrontBuffer = frontBuffer.lock())
		{
			ImGui::GetWindowDrawList()->AddImage(RenderingSystem::GetInstance()->GetRenderBufferColorApiID(*shFrontBuffer), pos, size, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			ImGui::End();
		}
		else
		{
			Log::Warning("ViewportWidget", "Invalid front buffer, widget will be empty");
		}
	}
}
