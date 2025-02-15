
#include "InputProviderGL.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Utils/log.h>

#include <imgui/backends/imgui_impl_glfw.h>
#include <glm/glm.hpp>

namespace GaladHen
{
	bool* InputProviderGL::KeyboardKeyStatus = nullptr;
	bool* InputProviderGL::MouseKeyStatus = nullptr;

	KeyboardKey InputProviderGL::KeyboardKeyAssociations[] = { KeyboardKey::Unknown };
	MouseKey InputProviderGL::MouseKeyAssociations[] = { MouseKey::Unknown };
	KeyAction InputProviderGL::KeyActionAssociations[] = {};

	InputProviderGL::InputProviderGL()
	{
		static bool Initialized = false;

		if (!Initialized)
		{
			FillKeyAssociations();

			Initialized = true;
		}
	}

	void InputProviderGL::RegisterKeyboardKeyStatus(bool* keyboardKeyStatus)
	{
		GLFWwindow* win = glfwGetCurrentContext();

		if (!win)
		{
			Log::Error("InputProviderGL", "Impossible to retrieve current GLFW window, keyboard key status not registered");

			return;
		}

		KeyboardKeyStatus = keyboardKeyStatus;

		glfwSetKeyCallback(win, &InputProviderGL::KeyboardKeyCallback);
	}

	void InputProviderGL::RegisterMouseKeyStatus(bool* mouseKeyStatus)
	{
		GLFWwindow* win = glfwGetCurrentContext();

		if (!win)
		{
			Log::Error("InputProviderGL", "Impossible to retrieve current GLFW window, mouse key status not registered");

			return;
		}

		MouseKeyStatus = mouseKeyStatus;

		glfwSetMouseButtonCallback(win, &InputProviderGL::MouseKeyCallback);
	}

	bool InputProviderGL::IsCloseWindowRequested()
	{
		GLFWwindow* win = glfwGetCurrentContext();
		return glfwWindowShouldClose(win);
	}

	void InputProviderGL::InvokePendingCallbacks()
	{
		glfwPollEvents();
	}

	glm::vec2 InputProviderGL::GetMouseCursorPosition()
	{
		GLFWwindow* win = glfwGetCurrentContext();
		double xpos, ypos;
		glfwGetCursorPos(win, &xpos, &ypos);

		return glm::vec2(xpos, ypos);
	}

	void InputProviderGL::FillKeyAssociations()
	{
		KeyboardKeyAssociations[GLFW_KEY_W] = KeyboardKey::W;
		KeyboardKeyAssociations[GLFW_KEY_A] = KeyboardKey::A;
		KeyboardKeyAssociations[GLFW_KEY_S] = KeyboardKey::S;
		KeyboardKeyAssociations[GLFW_KEY_D] = KeyboardKey::D;
		KeyboardKeyAssociations[GLFW_KEY_E] = KeyboardKey::E;
		KeyboardKeyAssociations[GLFW_KEY_Q] = KeyboardKey::Q;

		MouseKeyAssociations[GLFW_MOUSE_BUTTON_LEFT] = MouseKey::Left;
		MouseKeyAssociations[GLFW_MOUSE_BUTTON_RIGHT] = MouseKey::Right;

		KeyActionAssociations[GLFW_PRESS] = KeyAction::Press;
		KeyActionAssociations[GLFW_RELEASE] = KeyAction::Release;
		KeyActionAssociations[GLFW_REPEAT] = KeyAction::Press;
	}

	void InputProviderGL::KeyboardKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (KeyboardKeyStatus && key < GH_KEYBOARD_KEY_ASSOCIATIONS_NUMBER && action < GH_KEY_ACTION_ASSOCIATIONS_NUMBER)
		{
			// Check if key is associated with a GaladHen KeyboardKey
			if (KeyboardKeyAssociations[key] != KeyboardKey::Unknown)
			{
				KeyboardKeyStatus[(int)KeyboardKeyAssociations[key]] = (int)KeyActionAssociations[action];

				// Forward callback to ImGui
				ImGui_ImplGlfw_KeyCallback(window, key, 0, action, 0);
			}
		}
	}

	void InputProviderGL::MouseKeyCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (MouseKeyStatus && button < GH_MOUSE_KEY_ASSOCIATIONS_NUMBER && action < GH_KEY_ACTION_ASSOCIATIONS_NUMBER)
		{
			// Check if button is associated with a GaladHen MouseKey
			if (MouseKeyAssociations[button] != MouseKey::Unknown)
			{
				MouseKeyStatus[(int)MouseKeyAssociations[button]] = (int)KeyActionAssociations[action];

				// Forward callback to ImGui
				ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
			}
		}
	}
}
