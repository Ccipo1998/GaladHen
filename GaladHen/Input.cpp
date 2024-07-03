
#include "Input.h"
#include <Utils/Log.h>

#include <algorithm>

namespace GaladHen
{
    Input::Input()
        : MouseX(0.0f)
        , MouseY(0.0f)
        , LastMouseX(0.0f)
        , LastMouseY(0.0f)
    {
        // fill arrays
        std::fill_n(Keyboard, 1024, false);
        std::fill_n(Mouse, 12, false);
    }

    void Input::KeyboardCallback(void* sender, unsigned int key, unsigned int action)
    {
        if (key >= 1024)
        {
            Log::Error("Input", "Invalid keyboard key from callback");

            return;
        }

        if (action == (int)KeyAction::Pressed)
            Keyboard[key] = true;
        else if (action == (int)KeyAction::Released)
            Keyboard[key] = false;
    }

    void Input::MouseKeyCallback(void* sender, unsigned int key, unsigned int action)
    {
        if (action == (int)KeyAction::Pressed)
            Mouse[key] = true;
        else if (action == (int)KeyAction::Released)
            Mouse[key] = false;
    }

    void Input::MousePositionCallback(void* sender, float mouseX, float mouseY)
    {
        LastMouseX = MouseX;
        LastMouseY = MouseY;

        MouseX = mouseX;
        MouseY = mouseY;
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
}
