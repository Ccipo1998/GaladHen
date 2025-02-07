
#include "InspectorWidget.h"

#include <imgui/imgui.h>

#include <Systems/RenderingSystem/Entities/SceneObject.h>

namespace GaladHen
{
	UIInspectorWidget::UIInspectorWidget(const char* widgetName)
		: UIWidget(widgetName)
		{}

	void UIInspectorWidget::Build()
	{
		ImGui::Begin(WidgetName.data());

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Test");
		}

		ImGui::End();
	}

	void UIInspectorWidget::SetInspectedSceneObject(SceneObject* sceneObject)
	{
		if (sceneObject)
			InspectedSceneObject = std::make_shared<SceneObject>(*sceneObject);
	}
}
