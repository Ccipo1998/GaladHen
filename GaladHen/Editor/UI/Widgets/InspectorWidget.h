
// Inspector widget for a generic SceneObject

#pragma once

#include <memory>

#include <Systems/RenderingSystem/UI/Widget.h>

namespace GaladHen
{
	class SceneObject;

	class UIInspectorWidget : public UIWidget
	{
	public:

		UIInspectorWidget(const char* widgetName);

		virtual void Build() override;

		void SetInspectedSceneObject(std::weak_ptr<SceneObject> sceneObject);

	protected:

		std::weak_ptr<SceneObject> InspectedSceneObject;

	};
}
