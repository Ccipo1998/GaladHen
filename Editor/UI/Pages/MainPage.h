
// Main page

#pragma once

#include <Editor/UI/Page.h>
#include <Editor/UI/Widgets/NavBarWidget.h>
#include <Editor/UI/Widgets/AssetsViewWidget.h>

namespace GaladHen
{
	class UIMainPage : public UIPage
	{
	public:

		UIMainPage(const char* pageTitle, Window* ownerWindow);

		void BuildPage() override;

		UINavBarWidget NavBar;
		UIAssetsViewWidget AssetsView;
	};
}
