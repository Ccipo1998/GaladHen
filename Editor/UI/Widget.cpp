

#include "Widget.h"

namespace GaladHen
{
	UIWidget::UIWidget()
	{
		WidgetName = "";
	}

	UIWidget::UIWidget(const char* widgetName)
		: WidgetName(widgetName)
		{}

	void UIWidget::BuildWidget() {}

	UIWidget::~UIWidget() {}
}
