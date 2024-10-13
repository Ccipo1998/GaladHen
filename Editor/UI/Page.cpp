
#include "Page.h"
#include <Renderer/Window.h>

#include <Renderer/Window.h>
#include <Renderer/Common.h>

#include <Renderer/LayerAPI/OpenGL/WindowGL.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <Utils/Log.h>

namespace GaladHen
{
	UIPage::UIPage(const char* pageTitle, Window* ownerWindow)
		: PageTitle(pageTitle)
		, OwnerWindow(ownerWindow)
	{
		Init();
	}

	UIPage::UIPage(const UIPage& other) noexcept
	{
		OwnerWindow = other.OwnerWindow;
		PageTitle = other.PageTitle;

		Init();
	}

	UIPage& UIPage::operator=(const UIPage& other) noexcept
	{
		OwnerWindow = other.OwnerWindow;
		PageTitle = other.PageTitle;

		Init();

		return *this;
	}

	UIPage::UIPage(UIPage&& other) noexcept
	{
		Context = other.Context;
		OwnerWindow = other.OwnerWindow;
		PageTitle = other.PageTitle;

		Init();

		other.Context = nullptr;
		other.OwnerWindow = nullptr;
		other.PageTitle.clear();
	}

	UIPage& UIPage::operator=(UIPage&& other) noexcept
	{
		Context = other.Context;
		OwnerWindow = other.OwnerWindow;
		PageTitle = other.PageTitle;

		Init();

		other.Context = nullptr;
		other.OwnerWindow = nullptr;
		other.PageTitle.clear();

		return *this;
	}

	void UIPage::NewFrame()
	{
		SetContextCurrent();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void UIPage::BuildPage()
	{
		//ImGui::ShowDemoWindow();
		
		ImGui::Begin("Title");

		ImGui::Text("Test");

		ImGui::End();

	}

	void UIPage::Draw()
	{
		SetContextCurrent();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	Window* UIPage::GetOwningWindow()
	{
		return OwnerWindow;
	}

	void UIPage::SetContextCurrent()
	{
		ImGui::SetCurrentContext(Context);
	}

	void UIPage::SendKeyboardCallback(Window* owner, unsigned int key, unsigned int action)
	{
		switch (owner->GetWindowAPI())
		{
		default:
		case API::OpenGL:

			WindowGL* winGL = static_cast<WindowGL*>(owner->GetAPILevelWindow());
			ImGui_ImplGlfw_KeyCallback(winGL->GetGLFWWindow(), key, 0, action, 0);

			break;
		}
	}

	void UIPage::SendMouseKeyCallback(Window* owner, unsigned int key, unsigned int action)
	{
		switch (owner->GetWindowAPI())
		{
		default:
		case API::OpenGL:

			WindowGL* winGL = static_cast<WindowGL*>(owner->GetAPILevelWindow());
			ImGui_ImplGlfw_MouseButtonCallback(winGL->GetGLFWWindow(), key, action, 0);

			break;
		}
	}

	UIPage::~UIPage()
	{
		SetContextCurrent();
		
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void UIPage::CreateContext()
	{
		Context = (UIContext*)ImGui::CreateContext();
	}

	void UIPage::Init()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		switch (OwnerWindow->GetWindowAPI())
		{
		default:
		case (API::OpenGL):

			WindowGL* winGL = static_cast<WindowGL*>(OwnerWindow->GetAPILevelWindow());

			// Setup Platform/Renderer backends
			bool success = ImGui_ImplGlfw_InitForOpenGL(winGL->GetGLFWWindow(), true);
			success &= ImGui_ImplOpenGL3_Init(GLSL_VERSION);

			if (!success)
			{
				Log::Error("UIPage.cpp", "ImGui failed to setup backends for OpenGL and GLFW");
			}

			break;
		}
	}
}