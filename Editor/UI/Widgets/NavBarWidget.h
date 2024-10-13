
// Default view UI page

#pragma once

#include <Editor/UI/Widget.h>

namespace GaladHen
{
	class UINavBarWidget : public UIWidget
	{
	public:

		UINavBarWidget(const char* widgetName);

		virtual void BuildWidget() override;

	};
}
