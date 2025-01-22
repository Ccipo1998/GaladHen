
// Widget class, the building block of a UI Page

#pragma once

#include <string>

namespace GaladHen
{
	class UIPage;

	class UIWidget
	{
	public:

		UIWidget(const char* widgetName, UIPage* ownerPage);

		virtual void BuildWidget();

		~UIWidget();

		std::string WidgetName;

	protected:

		UIPage* OwnerPage;

	};
}
