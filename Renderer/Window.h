
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
        void SetKeyboardCallback(void (Input::*callback)(void* sender, unsigned int key, unsigned int action), Input* owner);

        // @brief
        // Set the callback to call when a mouse key is pressed or change status
        void SetMouseKeyCallback(void (Input::*callback)(void* sender, unsigned int key, unsigned int action), Input* owner);

        // @brief
        // Set the callback to call when the mouse is moved
        void SetMousePositionCallback(void (Input::* callback)(void* sender, float mouseX, float mouseY), Input* owner);

        // @brief
        // Manual call to keyboard callback on the owner
        // @param key: integer representing the activated key
        // @param action: the action type on the key
        void CallKeyboardCallback(unsigned int key, unsigned int action);

        // @brief
        // Manual call to mouse callback on the owner
        // @param key: integer representing the activated key
        // @param action: the action type on the key
        void CallMouseKeyCallback(unsigned int key, unsigned int action);

        // @brief
        // Manual call to mouse callback on the owner
        // @param key: integer representing the activated key
        // @param action: the action type on the key
        void CallMousePositionCallback(float mouseX, float mouseY);

        ~Window();

    protected:

        std::string WindowName;
        unsigned int Width;
        unsigned int Height;

        // pointer to api level window
        IWindowAPI* WinAPI;
        void (Input::*OutKeyboardCallback)(void* sender, unsigned int key, unsigned int action);
        void (Input::*OutMouseKeyCallback)(void* sender, unsigned int key, unsigned int action);
        void (Input::*OutMousePosCallback)(void* sender, float mouseX, float mouseY);
        Input* OutKeyboardCallbackOwner;
        Input* OutMouseKeyCallbackOwner;
        Input* OutMousePosCallbackOwner;

        static void KeyboardCallback(Window* owner, unsigned int key, unsigned int action);
        static void MouseKeyCallback(Window* owner, unsigned int key, unsigned int action);
        static void MousePosCallback(Window* owner, float mouseX, float mouseY);

    };
}
