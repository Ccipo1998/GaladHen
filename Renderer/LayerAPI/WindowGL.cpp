
#include "WindowGL.h"
#include <GaladHen/Window.h>
// glfw
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Utils/Log.h>

namespace GaladHen
{
    WindowGL::WindowGL()
        : WinGL(nullptr)
        , KeyboardCallback(nullptr)
        , MouseCallback(nullptr)
        , KeyboardCallbackOwner(nullptr)
        , MouseCallbackOwner(nullptr)
        {}

    WindowGL::WindowGL(unsigned int width, unsigned int height, const char* name)
        : KeyboardCallback(nullptr)
        , MouseCallback(nullptr)
        , KeyboardCallbackOwner(nullptr)
        , MouseCallbackOwner(nullptr)
    {
        InitContext(width, height, name);
    }

    void WindowGL::RegisterKeyboardCallback(void (Window::* callback)(unsigned int key, unsigned int action), Window* obj)
    {
        // save external callback and register wingl callback
        KeyboardCallback = callback;
        KeyboardCallbackOwner = obj;
        glfwSetKeyCallback(WinGL, WindowGL::KeyboardCallbackGL);
    }

    void WindowGL::RegisterMouseCallback(void (Window::* callback)(unsigned int key, unsigned int action), Window* obj)
    {
        // save external callback and register wingl callback
        MouseCallback = callback;
        MouseCallbackOwner = obj;
        glfwSetMouseButtonCallback(WinGL, WindowGL::MouseCallbackGL);
    }

    void WindowGL::GetCursorPosition(float& cursorX, float& cursorY)
    {
        double x, y;
        glfwGetCursorPos(WinGL, &x, &y);

        cursorX = static_cast<float>(x);
        cursorY = static_cast<float>(y);
    }

    void WindowGL::KeyboardCallbackGL(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        // translation from opengl callback to galadhen window callback
        (KeyboardCallbackOwner->*KeyboardCallback)(key, action);
    }

    void WindowGL::MouseCallbackGL(GLFWwindow* window, int button, int action, int mods)
    {
        // translation from opengl callback to galadhen window callback
        (MouseCallbackOwner->*MouseCallback)(button, action);
    }

    void WindowGL::InitContext(unsigned int width, unsigned int height, const char* name)
    {
        if (!glfwInit())
        {
            Log::Error("WindowGL", "GLFW failed to initialize the context");

            return;
        }

        // setting the minimum required version of OpenGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        // core profile is a subset of OpenGL features (without the backward-compatible features)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // for MacOS:
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        WinGL = glfwCreateWindow(width, height, name, nullptr, nullptr);
        if (WinGL == nullptr)
        {
            Log::Error("WindowGL", "GLFW failed to create the window");
        }
    }

    WindowGL::~WindowGL()
    {
        // TODO
    }
}


