
// UI Page class with UI functionalities and ImGui link

#pragma once

#include <imgui/imgui_internal.h>
#include <string>

// TODO: rendere api agnostic a runtime (virtual)

namespace GaladHen
{
	class Window;

	struct UIContext : ImGuiContext
	{
		// struct to (eventually) add data to context
	};

	class UIPage
	{

	public:

		UIPage(const char* pageTitle, Window* ownerWindow);

		// Copy constructor
		UIPage(const UIPage& other) noexcept;
		// Copy assignment
		UIPage& operator=(const UIPage& other) noexcept;
		// Move constructor
		UIPage(UIPage&& other) noexcept;
		// Move assignment
		UIPage& operator=(UIPage&& other) noexcept;

		virtual void NewFrame();

		virtual void Draw();

		virtual void BuildPage();

		Window* GetOwningWindow();

		void SetContextCurrent();

		// INPUT -----------------------------------------------------------

		void SendKeyboardCallback(Window* owner, unsigned int key, unsigned int action);

		void SendMouseKeyCallback(Window* owner, unsigned int key, unsigned int action);

		// INPUT -----------------------------------------------------------

		~UIPage();

	protected:

		void CreateContext();

		void Init();

		std::string PageTitle;
		Window* OwnerWindow;
		UIContext* Context;

	};
}