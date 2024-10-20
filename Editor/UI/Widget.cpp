

#include "Widget.h"

namespace GaladHen
{
	UIWidget::UIWidget(const char* widgetName, UIPage* ownerPage)
		: WidgetName(widgetName)
		, OwnerPage(ownerPage)
	{}

	void UIWidget::BuildWidget() {}

	UIWidget::~UIWidget() {}
}
