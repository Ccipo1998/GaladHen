
#include "WindowGL.h"

#include <Core/Input.h>

// glfw
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <Utils/Log.h>

#include <glm/glm.hpp>

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

        CreateOpenGLWindow(width, height, name);
    }

    GLFWwindow* WindowGL::GetGLFWWindow()
    {
        return WinGL;
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

    void WindowGL::RegisterClosingWindowCallback(void (*callback)(void* owner), void* owner)
    {
        ClosingWindowCallback = callback;
        ClosingWindowCallbackOwner = owner;
        glfwSetWindowUserPointer(WinGL, this); // to retrieve this pointer inside static function call
        glfwSetWindowCloseCallback(WinGL, WindowGL::ClosingWindowCallbackGL);
    }

    void WindowGL::InvokePendingCallbacks()
    {
        glfwPollEvents();
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
        winGL->SendMouseKeyCallback(ghKey, ghAction);
    }

    void WindowGL::MousePosCallbackGL(GLFWwindow* window, double xpos, double ypos)
    {
        // translation from opengl callback to galadhen window callback

        // get context
        WindowGL* winGL = static_cast<WindowGL*>(glfwGetWindowUserPointer(window));

        // translation
        float mouseX = static_cast<float>(xpos);
        float mouseY= static_cast<float>(ypos);

        // send callback
        winGL->SendMousePositionCallback(mouseX, mouseY);
    }

    void WindowGL::ClosingWindowCallbackGL(GLFWwindow* window)
    {
        // get context
        WindowGL* winGL = static_cast<WindowGL*>(glfwGetWindowUserPointer(window));

        winGL->SendClosingWindowCallback();
    }

    void WindowGL::CreateOpenGLWindow(unsigned int width, unsigned int height, const char* name)
    {
        WinGL = glfwCreateWindow(width, height, name, nullptr, nullptr);
        if (WinGL == nullptr)
        {
            Log::Error("WindowGL", "GLFW failed to create the window");
        }

        glfwMakeContextCurrent(WinGL);
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

    void WindowGL::SendClosingWindowCallback()
    {
        ClosingWindowCallback(ClosingWindowCallbackOwner);
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

    void WindowGL::ClearFrontBuffers(bool colorBuffer, bool depthBuffer, bool stencilBuffer)
    {
        GLbitfield mask = (colorBuffer ? GL_COLOR_BUFFER_BIT : 0) | (depthBuffer ? GL_DEPTH_BUFFER_BIT : 0) | (stencilBuffer ? GL_STENCIL_BUFFER_BIT : 0);
        glClear(mask);
    }

    void WindowGL::SwapBuffers()
    {
        glfwSwapBuffers(WinGL);
    }

    void WindowGL::SetColorBufferClearColor(glm::vec4 color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
    }

    void WindowGL::EnableDephtTest(bool enable)
    {
        if (enable)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }

    void WindowGL::CloseWindow()
    {
        glfwDestroyWindow(WinGL);
        glfwTerminate();
    }

    WindowGL::~WindowGL()
    {
        // TODO
    }
}


