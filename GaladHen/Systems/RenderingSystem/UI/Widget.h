
// Widget class, the building block of a UI Page

#pragma once

#include <string>

namespace GaladHen
{
	class UIPage;

	class UIWidget
	{
	public:

		UIWidget(const char* widgetName);

		virtual void Build();

		virtual ~UIWidget();

		std::string WidgetName;

	};
}
