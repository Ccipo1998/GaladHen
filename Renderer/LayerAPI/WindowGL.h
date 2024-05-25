
#pragma once

#include "IWindowAPI.h"

class GLFWwindow;

namespace GaladHen
{
    class Window;

    class WindowGL : public IWindowAPI
    {
        
    public:

        WindowGL();

        WindowGL(unsigned int width, unsigned int height, const char* name);

        WindowGL(const WindowGL& other) = delete; //disallow copy
        WindowGL& operator=(const WindowGL& other) = delete;

        virtual void RegisterKeyboardCallback(void (Window::*)(unsigned int key, unsigned int action), Window* obj) override;

        virtual void RegisterMouseCallback(void (Window::*)(unsigned int key, unsigned int action), Window* obj) override;

        virtual void GetCursorPosition(float& cursorX, float& cursorY) override;

        virtual ~WindowGL() override;

    protected:

        static void KeyboardCallbackGL(GLFWwindow* window, int key, int scancode, int action, int mode);
        static void MouseCallbackGL(GLFWwindow* window, int button, int action, int mods);

        // @brief
        // Initialization of the OpenGL context (glfw & gl3w) and of the window
        void InitContext(unsigned int width, unsigned int height, const char* name);

        GLFWwindow* WinGL;
        Window* KeyboardCallbackOwner;
        void (Window::* KeyboardCallback)(unsigned int key, unsigned int action);
        Window* MouseCallbackOwner;
        void (Window::* MouseCallback)(unsigned int key, unsigned int action);

    };
}
