
// Main nav bar widget

#pragma once

#include <Systems/RenderingSystem/UI/Widget.h>

namespace GaladHen
{
	class UINavBarWidget : public UIWidget
	{
	public:

		UINavBarWidget(const char* widgetName);

		virtual void Build() override;

	};
}
