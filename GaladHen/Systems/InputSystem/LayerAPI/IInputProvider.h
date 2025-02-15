
// This is a pure virtual class (interface) for api-specific input providers
// An input provider is a class which provides input callbacks to the higher level InputSystem

#pragma once

#include <glm/fwd.hpp>

namespace GaladHen
{
	enum class KeyboardKey;
	enum class MouseKey;
	enum class KeyAction;

	class IInputProvider
	{
	public:

		virtual void RegisterKeyboardKeyStatus(bool* keyboardKeyStatus) = 0;
		virtual void RegisterMouseKeyStatus(bool* mouseKeyStatus) = 0;
		virtual bool IsCloseWindowRequested() = 0;
		virtual void InvokePendingCallbacks() = 0;
		virtual glm::vec2 GetMouseCursorPosition() = 0;
	};
}
