
// Widget class, the building block of a UI Page

#pragma once

#include <string>

class UINode
{

};

namespace GaladHen
{
	class UIPage;

	class UIWidget
	{
	public:

		UIWidget(const char* widgetName, UIPage* ownerPage);

		virtual void BuildWidget();

		~UIWidget();

		unsigned int WidgetID;
		std::string WidgetName;

	protected:

		UIPage* OwnerPage;

	};
}
