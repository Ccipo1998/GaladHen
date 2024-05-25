
#include "Input.h"
#include "Window.h"
#include <Utils/Log.h>

namespace GaladHen
{
    Input::Input()
        : MouseX(0.0f)
        , MouseY(0.0f)
        , LastMouseX(0.0f)
        , LastMouseY(0.0f)
        {}

    void Input::LinkInputCallbacks(Window* window)
    {
        if (window == nullptr)
        {
            Log::Error("Input", "Tried to link input callbacks to null window");

            return;
        }

        window->SetKeyboardCallback(&Input::KeyboardCallback, this);
        window->SetMouseCallback(&Input::MouseCallback, this);
    }

    void Input::Update(Window* window)
    {
        if (window == nullptr)
        {
            Log::Error("Input", "Tried to update input from a null window");

            return;
        }

        // update mouse position
        LastMouseX = MouseX;
        LastMouseY = MouseY;
        window->GetCurrentMousePosition(MouseX, MouseY);
    }

    bool Input::GetKeyboardKey(unsigned int key)
    {
        if (key >= 1024)
        {
            Log::Error("Input", "Tried to access keyboard array with an invalid index");

            return false;
        }

        return Keyboard[key];
    }

    bool Input::GetMouseKey(unsigned int key)
    {
        if (key >= 12)
        {
            Log::Error("Input", "Tried to access mouse array with an invalid index");

            return false;
        }

        return Mouse[key];
    }

    float Input::GetMouseX()
    {
        return MouseX;
    }

    float Input::GetMouseY()
    {
        return MouseY;
    }

    float Input::GetDeltaMouseX()
    {
        return MouseX - LastMouseX;
    }

    float Input::GetDeltaMouseY()
    {
        return MouseY - LastMouseY;
    }

    void Input::KeyboardCallback(Window* window, unsigned int key, unsigned int action)
    {
        if (key >= 1024)
        {
            Log::Error("Input", "Invalid keyboard key from callback");

            return;
        }

        if(action == KeyAction::Pressed)
            Keyboard[key] = true;
        else if(action == KeyAction::Released)
            Keyboard[key] = false;
    }

    void Input::MouseCallback(Window* window, unsigned int key, unsigned int action)
    {
        if(action == KeyAction::Pressed)
            Mouse[key] = true;
        else if(action == KeyAction::Released)
            Mouse[key] = false;
    }
}
