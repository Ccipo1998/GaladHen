
#pragma once

#include <string>

#include <glm/glm.hpp>
#include "Common.h"
#include <Core/Input.h>

namespace GaladHen
{
    class IWindowAPI;

    class Window
    {

    public:

        Window(API apiToUse);
        
        Window(API apiToUse, const std::string& windowName);

        Window(API apiToUse, const std::string& windowName, unsigned int width, unsigned int height);

        // WINDOW ----------------------------------------------------------------------------------------------------------------

        float GetAspectRatio();

        void CloseWindow();

        // void ChangeWindowName(const std::string& windowName);

        // void ChangeWindowSize(unsigned int width, unsigned int height);

        // INPUT -------------------------------------------------------------------------------------------------------------------

        // @brief
        // Manual call to keyboard callback
        // @param key: the activated key
        // @param action: the action type on the key
        void CallKeyboardCallback(KeyboardKey key, KeyAction action);

        // @brief
        // Manual call to mouse callback
        // @param key: the activated key
        // @param action: the action type on the key
        void CallMouseKeyCallback(MouseKey key, KeyAction action);

        // @brief
        // Manual call to mouse callback on the owner
        // @param key: integer representing the activated key
        // @param action: the action type on the key
        void CallMousePositionCallback(float mouseX, float mouseY);

        // @brief
        // Manual call to closing window callback on the owner
        void CallClosingWindowCallback();

        // @brief
        // Check if a keyboard key is currently pressed
        bool IsKeyPressed(KeyboardKey key);

        // @brief
        // Check if a mouse key is currently pressed
        bool IsKeyPressed(MouseKey key);

        // @brief
        // Get current mouse position
        void GetMousePosition(float& mouseX, float& mouseY);

        void GetMousePositionDelta(float& deltaX, float& deltaY);

        bool IsCloseWindowRequested();

        // RENDERING ------------------------------------------------------------------------------------------------------

        // @brief
        // Perform operations needed at the beginning of a frame
        void BeginFrame();

        // @brief
        // Perform operations needed at the end of a frame
        void EndFrame();

        void SetColorBufferClearColor(glm::vec4 color);

        ~Window();

    protected:

        std::string WindowName;
        unsigned int Width;
        unsigned int Height;

        Input WindowInput;

        // pointer to api level window
        IWindowAPI* WinAPI;

        void RegisterInputCallbacks();

        static void KeyboardCallback(Window* owner, unsigned int key, unsigned int action);
        static void MouseKeyCallback(Window* owner, unsigned int key, unsigned int action);
        static void MousePosCallback(Window* owner, float mouseX, float mouseY);
        static void ClosingWindowCallback(Window* owner);

    };
}
