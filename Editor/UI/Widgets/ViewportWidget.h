
#pragma once

#include <Editor/UI/Widget.h>

#include <glm/glm.hpp>

namespace GaladHen
{
	class UIViewportWidget : public UIWidget
	{
	public:

		UIViewportWidget(const char* widgetName, UIPage* ownerPage);

		virtual void BuildWidget() override;

	};
}

