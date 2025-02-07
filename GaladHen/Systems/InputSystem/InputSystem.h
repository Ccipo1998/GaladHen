
#pragma once

#include <Systems/ISystem.h>
#include <Utils/WeakSingleton.hpp>
#include <glm/glm.hpp>
#include <Systems/RenderingSystem/Common.h>
#include <Systems/SystemsCoordinator.h>

#define GH_KEYBOARD_KEY_NUMBER 4
#define GH_MOUSE_KEY_NUMBER 3

namespace GaladHen
{
	enum class KeyboardKey;
	enum class MouseKey;
	enum class KeyAction;
	class IInputProvider;

	class InputSystem : public ISystem, public WeakSingleton<InputSystem>
	{
		friend SystemsCoordinator;

	public:

		InputSystem();

		void RegisterToInputProvider();

		bool IsCloseWindowRequested();
		void InvokePendingCallbacks();
		bool IsKeyboardKeyPressed(KeyboardKey key);
		bool IsMouseKeyPressed(MouseKey key);
		glm::uvec2 GetMousePosition();

	private:

		virtual void Init() override;

		virtual void Quit() override;

		bool KeyboardKeyStatus[GH_KEYBOARD_KEY_NUMBER] = { false };
		bool MouseKeyStatus[GH_MOUSE_KEY_NUMBER] = { false };

		API CurrentAPI;
		IInputProvider* InputProviderAPI;
	};
}
