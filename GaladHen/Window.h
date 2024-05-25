
#pragma once

#include <string>

namespace GaladHen
{
    class Input;
    class IWindowAPI;

    class Window
    {

    public:

        Window();

        Window(const std::string& windowName, unsigned int width, unsigned int height);

        float GetAspectRatio();

        // void ChangeWindowName(const std::string& windowName);

        // void ChangeWindowSize(unsigned int width, unsigned int height);

        // @brief
        // Set the callback to call when a key is pressed or change status
        void SetKeyboardCallback(void (Input::*callback)(Window*, unsigned int key, unsigned int action), Input* obj);

        // @brief
        // Set the callback to call when a mouse key is pressed or change status
        void SetMouseCallback(void (Input::*callback)(Window*, unsigned int key, unsigned int action), Input* obj);

        // @brief
        // Write on mouseX and mouseY references the current values of mouse position in viewport space
        void GetCurrentMousePosition(float& mouseX, float& mouseY);

        ~Window();

    protected:

        std::string WindowName;
        unsigned int Width;
        unsigned int Height;

        // pointer to api level window
        IWindowAPI* WinAPI;
        void (Input::*OutKeyboardCallback)(Window*, unsigned int key, unsigned int action);
        void (Input::*OutMouseCallback)(Window*, unsigned int key, unsigned int action);
        Input* OutKeyboardCallbackOwner;
        Input* OutMouseCallbackOwner;

        void KeyboardCallback(unsigned int key, unsigned int action);
        void MouseCallback(unsigned int key, unsigned int action);

    };
}
