
#pragma once

#include <string>

#include <glm/glm.hpp>
#include "Common.h"

namespace GaladHen
{
    class IWindowAPI;
    class UIPage;

    struct Input
    {
        enum class KeyboardKey
        {
            W = 0,
            A = 1,
            S = 2,
            D = 3,
            E = 4,
            Q = 5
        };

        enum class MouseKey
        {
            Left = 0,
            Right = 1
        };

        enum class KeyAction
        {
            Released = 0,
            Pressed = 1
        };

        // array of booleans for each keybord key
        bool Keyboard[1024];

        // array for mouse buttons
        bool Mouse[12];

        // mouse coordinates
        float MouseX;
        float MouseY;
        float LastMouseX;
        float LastMouseY;

        bool CloseWindowRequested;
    };

    class Window
    {

    public:

        Window();

        Window(const Window& other) = delete;
        Window& operator=(const Window& other) = delete;
        Window& operator=(Window&& other) noexcept;
        Window(Window&& other) noexcept;

        // WINDOW ----------------------------------------------------------------------------------------------------------------

        void Init(API apiToUse);

        void Init(API apiToUse, const std::string& windowName);

        void Init(API apiToUse, const std::string& windowName, unsigned int width, unsigned int height);

        float GetAspectRatio();

        void CloseWindow();

        API GetWindowAPI() const;

        IWindowAPI* GetAPILevelWindow();

        glm::uvec2 GetWindowPosition();

        glm::uvec2 GetWindowSize();

        // void ChangeWindowName(const std::string& windowName);

        // void ChangeWindowSize(unsigned int width, unsigned int height);

        // INPUT -------------------------------------------------------------------------------------------------------------------

        // @brief
        // Manual call to keyboard callback
        // @param key: the activated key
        // @param action: the action type on the key
        void CallKeyboardCallback(Input::KeyboardKey key, Input::KeyAction action);

        // @brief
        // Manual call to mouse callback
        // @param key: the activated key
        // @param action: the action type on the key
        void CallMouseKeyCallback(Input::MouseKey key, Input::KeyAction action);

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
        bool IsKeyPressed(Input::KeyboardKey key);

        // @brief
        // Check if a mouse key is currently pressed
        bool IsKeyPressed(Input::MouseKey key);

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

        ~Window();

    protected:

        std::string WindowName;
        unsigned int Width;
        unsigned int Height;

        API CurrentAPI;

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
