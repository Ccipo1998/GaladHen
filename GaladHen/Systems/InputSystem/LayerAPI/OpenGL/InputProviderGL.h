
#pragma once

#include <Systems/InputSystem/LayerAPI/IInputProvider.h>
#include <Systems/InputSystem/Input.h>

#define GH_KEYBOARD_KEY_ASSOCIATIONS_NUMBER 350
#define GH_MOUSE_KEY_ASSOCIATIONS_NUMBER 8
#define GH_KEY_ACTION_ASSOCIATIONS_NUMBER 4

struct GLFWwindow;

namespace GaladHen
{
	class InputProviderGL : public IInputProvider
	{
	public:

		InputProviderGL();

		virtual void RegisterKeyboardKeyStatus(bool* keyboardKeyStatus) override;
		virtual void RegisterMouseKeyStatus(bool* mouseKeyStatus) override;
		virtual bool IsCloseWindowRequested() override;
		virtual void InvokePendingCallbacks() override;

	protected:

		void FillKeyAssociations();

		static void KeyboardKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseKeyCallback(GLFWwindow* window, int button, int action, int mods);

		static KeyboardKey KeyboardKeyAssociations[GH_KEYBOARD_KEY_ASSOCIATIONS_NUMBER]; // https://www.glfw.org/docs/3.3/group__keys.html
		static MouseKey MouseKeyAssociations[GH_MOUSE_KEY_ASSOCIATIONS_NUMBER]; // https://www.glfw.org/docs/3.3/group__buttons.html
		static KeyAction KeyActionAssociations[GH_KEY_ACTION_ASSOCIATIONS_NUMBER]; // https://www.glfw.org/docs/3.3/group__input.html

		static bool* KeyboardKeyStatus;
		static bool* MouseKeyStatus;
	};
}
