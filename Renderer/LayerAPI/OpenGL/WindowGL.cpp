
#include "WindowGL.h"
#include <GaladHen/Input.h>
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
    {
        FillKeyAssociations();
    }

    WindowGL::WindowGL(unsigned int width, unsigned int height, const char* name)
        : KeyboardCallback(nullptr)
        , MouseCallback(nullptr)
        , KeyboardCallbackOwner(nullptr)
        , MouseCallbackOwner(nullptr)
    {
        FillKeyAssociations();

        InitContext(width, height, name);
    }

    void WindowGL::FillKeyAssociations()
    {
        KeyboardKeyAssociations[GLFW_KEY_W] = KeyboardKey::W;
        KeyboardKeyAssociations[GLFW_KEY_A] = KeyboardKey::A;
        KeyboardKeyAssociations[GLFW_KEY_S] = KeyboardKey::S;
        KeyboardKeyAssociations[GLFW_KEY_D] = KeyboardKey::D;
        KeyboardKeyAssociations[GLFW_KEY_E] = KeyboardKey::E;
        KeyboardKeyAssociations[GLFW_KEY_Q] = KeyboardKey::Q;
        
        MouseKeyAssociations[GLFW_MOUSE_BUTTON_LEFT] = MouseKey::Left;
        MouseKeyAssociations[GLFW_MOUSE_BUTTON_RIGHT] = MouseKey::Right;

        KeyActionAssociations[GLFW_PRESS] = KeyAction::Pressed;
        KeyActionAssociations[GLFW_RELEASE] = KeyAction::Released;
    }

    void WindowGL::RegisterKeyboardCallback(void (*callback)(void* owner, unsigned int key, unsigned int action), void* owner)
    {
        // save external callback and register wingl callback
        KeyboardCallback = callback;
        KeyboardCallbackOwner = owner;
        glfwSetWindowUserPointer(WinGL, this); // to retrieve this pointer inside static function call
        glfwSetKeyCallback(WinGL, WindowGL::KeyboardCallbackGL);
    }

    void WindowGL::RegisterMouseCallback(void (*callback)(void* owner, unsigned int key, unsigned int action), void* owner)
    {
        // save external callback and register wingl callback
        MouseCallback = callback;
        MouseCallbackOwner = owner;
        glfwSetWindowUserPointer(WinGL, this); // to retrieve this pointer inside static function call
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

        // get context
        WindowGL* winGL = static_cast<WindowGL*>(glfwGetWindowUserPointer(window));

        // translation
        unsigned int ghKey = winGL->TranslateKeyboardKey(key);
        unsigned int ghAction = winGL->TranslateKeyAction(action);

        // send callback
        winGL->SendKeyboardCallback(ghKey, ghAction);
    }

    void WindowGL::MouseCallbackGL(GLFWwindow* window, int button, int action, int mods)
    {
        // translation from opengl callback to galadhen window callback

        // get context
        WindowGL* winGL = static_cast<WindowGL*>(glfwGetWindowUserPointer(window));

        // translation
        unsigned int ghKey = winGL->TranslateMouseKey(button);
        unsigned int ghAction = winGL->TranslateKeyAction(action);

        // send callback
        winGL->SendKeyboardCallback(ghKey, ghAction);
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

    void WindowGL::SendKeyboardCallback(unsigned int key, unsigned int action)
    {
        KeyboardCallback(KeyboardCallbackOwner, key, action);
    }

    void WindowGL::SendMouseCallback(unsigned int key, unsigned int action)
    {
        MouseCallback(MouseCallbackOwner, key, action);
    }

    unsigned int WindowGL::TranslateKeyboardKey(int glSpecificKey)
    {
        return KeyboardKeyAssociations[glSpecificKey];
    }

    unsigned int WindowGL::TranslateMouseKey(int glSpecificKey)
    {
        return MouseKeyAssociations[glSpecificKey];
    }

    unsigned int WindowGL::TranslateKeyAction(int glSpecificAction)
    {
        return KeyActionAssociations[glSpecificAction];
    }

    WindowGL::~WindowGL()
    {
        // TODO
    }
}


