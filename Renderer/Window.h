
#pragma once

#include <string>

#include <glm/glm.hpp>
#include "Common.h"
#include <Core/Input.h>

namespace GaladHen
{
    class IWindowAPI;
    class UIPage;

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

        void EnableDepthTest(bool enable);

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
