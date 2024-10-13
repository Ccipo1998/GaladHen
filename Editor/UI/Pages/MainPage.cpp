
#include "MainPage.h"

namespace GaladHen
{
	UIMainPage::UIMainPage(const char* pageTitle, Window* ownerWindow)
		: UIPage(pageTitle, ownerWindow)
		, NavBar("NavBar")
		{}

	void UIMainPage::BuildPage()
	{
		NavBar.BuildWidget();
	}
}
