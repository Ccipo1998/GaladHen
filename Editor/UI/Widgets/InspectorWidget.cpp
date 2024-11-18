
#include "InspectorWidget.h"

#include <Editor/UI/Page.h>
#include <imgui/imgui.h>

#include <Core/SceneObject.h>

namespace GaladHen
{
	UIInspectorWidget::UIInspectorWidget(const char* widgetName, UIPage* ownerPage)
		: UIWidget(widgetName, ownerPage)
		{}

	void UIInspectorWidget::BuildWidget()
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
