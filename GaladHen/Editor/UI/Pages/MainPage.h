
// Main page

#pragma once

#include <Systems/RenderingSystem/UI/Page.h>

namespace GaladHen
{
	class UIMainPage : public UIPage
	{
	public:

		UIMainPage(const char* pageName);

		void Build() override;
	};
}
