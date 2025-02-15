
#include "InputSystem.h"
#include "LayerAPI/OpenGL/InputProviderGL.h"

namespace GaladHen
{
	InputSystem::InputSystem()
		: CurrentAPI(GH_CURRENT_API)
		, InputProviderAPI(nullptr)
	{}

	void InputSystem::RegisterToInputProvider()
	{
		// Create input provider, if null
		if (!InputProviderAPI)
		{
			switch (CurrentAPI)
			{
			case GaladHen::API::OpenGL:

				InputProviderAPI = new InputProviderGL{};

				break;
			default:
				break;
			}
		}

		InputProviderAPI->RegisterKeyboardKeyStatus(KeyboardKeyStatus);
		InputProviderAPI->RegisterMouseKeyStatus(MouseKeyStatus);
	}

	bool InputSystem::IsCloseWindowRequested()
	{
		return InputProviderAPI ? InputProviderAPI->IsCloseWindowRequested() : false;
	}

	void InputSystem::InvokePendingCallbacks()
	{
		if (InputProviderAPI)
		{
			InputProviderAPI->InvokePendingCallbacks();
		}
	}

	bool InputSystem::IsKeyboardKeyPressed(KeyboardKey key)
	{
		return KeyboardKeyStatus[(unsigned int)key];
	}

	bool InputSystem::IsMouseKeyPressed(MouseKey key)
	{
		return MouseKeyStatus[(unsigned int)key];
	}

	void InputSystem::UpdateMousePosition()
	{
		if (InputProviderAPI)
		{
			LastMousePosition = MousePosition;
			MousePosition = InputProviderAPI->GetMouseCursorPosition();
		}
	}

	glm::vec2 InputSystem::GetMousePosition()
	{
		return MousePosition;
	}

	glm::vec2 InputSystem::GetDeltaMousePosition()
	{
		return MousePosition - LastMousePosition;
	}

	void InputSystem::Init()
	{

	}

	void InputSystem::Quit()
	{
		if (InputProviderAPI)
			delete InputProviderAPI;
	}
}
