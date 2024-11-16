
// UI Page class with UI functionalities and ImGui link

#pragma once

#include <imgui/imgui_internal.h>
#include <string>
#include <vector>

#include <glm/fwd.hpp>

// TODO: rendere api agnostic a runtime (virtual)

namespace GaladHen
{
	class Window;
	class UIWidget;

	struct UIContext : ImGuiContext
	{
		// struct to (eventually) add data to context
	};

	class UIPage
	{

	public:

		UIPage(const char* pageName, Window* ownerWindow);

		// Copy constructor
		UIPage(const UIPage& other) noexcept;
		// Copy assignment
		UIPage& operator=(const UIPage& other) noexcept;
		// Move constructor
		UIPage(UIPage&& other) noexcept;
		// Move assignment
		UIPage& operator=(UIPage&& other) noexcept;

		virtual void NewFrame();

		virtual void BuildPage();

		virtual void Draw();

		Window* GetOwningWindow();

		void SetContextCurrent();

		// INPUT -----------------------------------------------------------

		void SendKeyboardCallback(Window* owner, unsigned int key, unsigned int action);

		void SendMouseKeyCallback(Window* owner, unsigned int key, unsigned int action);

		// INPUT -----------------------------------------------------------

		std::string PageName;
		std::vector<UIWidget*> Widgets;

		~UIPage();

	protected:

		void BuildWidgets();

		void CreateContext();

		void Init();

		Window* OwnerWindow;
		UIContext* Context;

	};
}