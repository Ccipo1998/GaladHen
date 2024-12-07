
// Inspector widget for a generic SceneObject

#pragma once

#include <memory>

#include <Editor/UI/Widget.h>

namespace GaladHen
{
	class SceneObject;

	class UIInspectorWidget : public UIWidget
	{
	public:

		UIInspectorWidget(const char* widgetName, UIPage* ownerPage);

		virtual void BuildWidget() override;

		void SetInspectedSceneObject(SceneObject* sceneObject);

	protected:

		std::weak_ptr<SceneObject> InspectedSceneObject;

	};
}