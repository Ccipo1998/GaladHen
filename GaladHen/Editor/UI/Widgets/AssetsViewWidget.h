
// Widget for main view of loaded assets

#pragma once

#include <Systems/RenderingSystem/UI/Widget.h>

namespace GaladHen
{
	class UIAssetsViewWidget : public UIWidget
	{
	public:

		UIAssetsViewWidget(const char* widgetName);

		virtual void Build() override;

	};
}
