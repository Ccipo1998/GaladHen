
#pragma once

#include <Systems/RenderingSystem/UI/Widget.h>

#include <glm/glm.hpp>

namespace GaladHen
{
	class UIViewportWidget : public UIWidget
	{
	public:

		UIViewportWidget(const char* widgetName);

		virtual void Build() override;

	};
}

