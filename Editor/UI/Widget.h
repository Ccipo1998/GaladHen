
// Widget class, the building block of a UI Page

#pragma once

#include <string>

namespace GaladHen
{
	class UIPage;

	class UIWidget
	{
	public:

		UIWidget();

		UIWidget(const char* widgetName);

		virtual void BuildWidget();

		~UIWidget();

		unsigned int WidgetID;
		std::string WidgetName;
	};
}
