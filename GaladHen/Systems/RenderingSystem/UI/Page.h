
// UI Page class with UI functionalities and ImGui link

#pragma once

#include <string>
#include <vector>

#include <glm/fwd.hpp>

namespace GaladHen
{
	class UIWidget;

	class UIPage
	{

	public:

		UIPage(const char* pageName);

		// Copy constructor
		UIPage(const UIPage& other) noexcept;
		// Copy assignment
		UIPage& operator=(const UIPage& other) noexcept;
		// Move constructor
		UIPage(UIPage&& other) noexcept;
		// Move assignment
		UIPage& operator=(UIPage&& other) noexcept;

		virtual void Build();

		std::string PageName;
		std::vector<UIWidget*> Widgets;

		virtual ~UIPage();

	protected:

		void BuildWidgets();

	};
}