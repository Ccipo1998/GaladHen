
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
        , MouseKeyCallback(nullptr)
        , MousePosCallback(nullptr)
        , KeyboardCallbackOwner(nullptr)
        , MouseKeyCallbackOwner(nullptr)
        , MousePosCallbackOwner(nullptr)
    {
        FillKeyAssociations();
    }

    WindowGL::WindowGL(unsigned int width, unsigned int height, const char* name)
        : KeyboardCallback(nullptr)
        , MouseKeyCallback(nullptr)
        , MousePosCallback(nullptr)
        , KeyboardCallbackOwner(nullptr)
        , MouseKeyCallbackOwner(nullptr)
        , MousePosCallbackOwner(nullptr)
    {
        FillKeyAssociations();

        InitContext(width, height, name);
    }

    void WindowGL::FillKeyAssociations()
    {
        KeyboardKeyAssociations[GLFW_KEY_W] = (int)KeyboardKey::W;
        KeyboardKeyAssociations[GLFW_KEY_A] = (int)KeyboardKey::A;
        KeyboardKeyAssociations[GLFW_KEY_S] = (int)KeyboardKey::S;
        KeyboardKeyAssociations[GLFW_KEY_D] = (int)KeyboardKey::D;
        KeyboardKeyAssociations[GLFW_KEY_E] = (int)KeyboardKey::E;
        KeyboardKeyAssociations[GLFW_KEY_Q] = (int)KeyboardKey::Q;
        
        MouseKeyAssociations[GLFW_MOUSE_BUTTON_LEFT] = (int)MouseKey::Left;
        MouseKeyAssociations[GLFW_MOUSE_BUTTON_RIGHT] = (int)MouseKey::Right;

        KeyActionAssociations[GLFW_PRESS] = (int)KeyAction::Pressed;
        KeyActionAssociations[GLFW_RELEASE] = (int)KeyAction::Released;
    }

    void WindowGL::RegisterKeyboardCallback(void (*callback)(void* owner, unsigned int key, unsigned int action), void* owner)
    {
        // save external callback and register wingl callback
        KeyboardCallback = callback;
        KeyboardCallbackOwner = owner;
        glfwSetWindowUserPointer(WinGL, this); // to retrieve this pointer inside static function call
        glfwSetKeyCallback(WinGL, WindowGL::KeyboardCallbackGL);
    }

    void WindowGL::RegisterMouseKeyCallback(void (*callback)(void* owner, unsigned int key, unsigned int action), void* owner)
    {
        // save external callback and register wingl callback
        MouseKeyCallback = callback;
        MouseKeyCallbackOwner = owner;
        glfwSetWindowUserPointer(WinGL, this); // to retrieve this pointer inside static function call
        glfwSetMouseButtonCallback(WinGL, WindowGL::MouseKeyCallbackGL);
    }

    void WindowGL::RegisterMousePositionCallback(void (*callback)(void* owner, float mouseX, float mouseY), void* owner)
    {
        // save external callback and register wingl callback
        MousePosCallback = callback;
        MousePosCallbackOwner = owner;
        glfwSetWindowUserPointer(WinGL, this); // to retrieve this pointer inside static function call
        glfwSetCursorPosCallback(WinGL, WindowGL::MousePosCallbackGL);
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

    void WindowGL::MouseKeyCallbackGL(GLFWwindow* window, int button, int action, int mods)
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

    void WindowGL::MousePosCallbackGL(GLFWwindow* window, double xpos, double ypos)
    {
        // translation from opengl callback to galadhen window callback

        // get context
        WindowGL* winGL = static_cast<WindowGL*>(glfwGetWindowUserPointer(window));

        // translation
        float mouseX = float(xpos);
        float mouseY= float(ypos);

        // send callback
        winGL->SendMousePositionCallback(mouseX, mouseY);
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

    void WindowGL::SendMouseKeyCallback(unsigned int key, unsigned int action)
    {
        MouseKeyCallback(MouseKeyCallbackOwner, key, action);
    }

    void WindowGL::SendMousePositionCallback(float mouseX, float mouseY)
    {
        MousePosCallback(MousePosCallbackOwner, mouseX, mouseY);
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


