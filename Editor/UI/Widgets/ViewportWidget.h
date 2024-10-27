
#pragma once

#include <Editor/UI/Widget.h>

#include <glm/fwd.hpp>

namespace GaladHen
{
	class UIViewportWidget : public UIWidget
	{
	public:

		UIViewportWidget(const char* widgetName, UIPage* ownerPage);

		virtual void BuildWidget() override;

		glm::vec2 GetViewportPosition();

		glm::vec2 GetViewportSize();

	protected:

		

	};
}

