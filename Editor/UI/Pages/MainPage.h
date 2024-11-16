
// Main page

#pragma once

#include <Editor/UI/Page.h>

namespace GaladHen
{
	class UIMainPage : public UIPage
	{
	public:

		UIMainPage(const char* pageName, Window* ownerWindow);

		void BuildPage() override;
	};
}
