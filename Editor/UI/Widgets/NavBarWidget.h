
// Main nav bar widget

#pragma once

#include <Editor/UI/Widget.h>

namespace GaladHen
{
	class UINavBarWidget : public UIWidget
	{
	public:

		UINavBarWidget(const char* widgetName, UIPage* ownerPage);

		virtual void BuildWidget() override;

		static unsigned int GetNavBarHeight();

	protected:

		static unsigned int Height;

	};
}
