
#pragma once

#include <string>

namespace GaladHen
{
    class IWindowAPI;
    class Input;

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
        void SetKeyboardCallback(void (Input::*callback)(Window* sender, unsigned int key, unsigned int action), Input* owner);

        // @brief
        // Set the callback to call when a mouse key is pressed or change status
        void SetMouseCallback(void (Input::*callback)(Window* sender, unsigned int key, unsigned int action), Input* owner);

        // @brief
        // Write on mouseX and mouseY references the current values of mouse position in viewport space
        void GetCurrentMousePosition(float& mouseX, float& mouseY);

        // @brief
        // Manual call to keyboard callback on the owner
        // @param key: integer representing the activated key
        // @param action: the action type on the key
        void CallKeyboardCallback(unsigned int key, unsigned int action);

        // @brief
        // Manual call to mouse callback on the owner
        // @param key: integer representing the activated key
        // @param action: the action type on the key
        void CallMouseCallback(unsigned int key, unsigned int action);

        ~Window();

    protected:

        std::string WindowName;
        unsigned int Width;
        unsigned int Height;

        // pointer to api level window
        IWindowAPI* WinAPI;
        void (Input::*OutKeyboardCallback)(Window* sender, unsigned int key, unsigned int action);
        void (Input::*OutMouseCallback)(Window* sender, unsigned int key, unsigned int action);
        Input* OutKeyboardCallbackOwner;
        Input* OutMouseCallbackOwner;

        static void KeyboardCallback(Window* owner, unsigned int key, unsigned int action);
        static void MouseCallback(Window* owner, unsigned int key, unsigned int action);

    };
}
