
// Widget for main view of loaded assets

#pragma once

#include <Editor/UI/Widget.h>

namespace GaladHen
{
	class UIAssetsViewWidget : public UIWidget
	{
	public:

		UIAssetsViewWidget(const char* widgetName, UIPage* ownerPage);

		virtual void BuildWidget() override;

	};
}
