
#include "Page.h"
#include "Widget.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace GaladHen
{
	UIPage::UIPage(const char* pageName)
		: PageName(pageName)
	{}

	UIPage::UIPage(const UIPage& other) noexcept
	{
		PageName = other.PageName;
	}

	UIPage& UIPage::operator=(const UIPage& other) noexcept
	{
		PageName = other.PageName;

		return *this;
	}

	UIPage::UIPage(UIPage&& other) noexcept
	{
		PageName = other.PageName;

		other.PageName.clear();
	}

	UIPage& UIPage::operator=(UIPage&& other) noexcept
	{
		PageName = other.PageName;

		other.PageName.clear();

		return *this;
	}

	void UIPage::Build()
	{
		BuildWidgets();
	}

	UIPage::~UIPage()
	{
		for (UIWidget* widget : Widgets)
		{
			delete widget;
		}
	}

	void UIPage::BuildWidgets()
	{
		// Call build page on each UI widget
		for (UIWidget* widget : Widgets)
		{
			widget->Build();
		}
	}
}