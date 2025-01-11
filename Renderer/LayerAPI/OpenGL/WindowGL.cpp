
#include "WindowGL.h"

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

// glfw
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <Utils/Log.h>
#include <Renderer/Common.h>
#include <glm/glm.hpp>
#include <Renderer/Window.h>

namespace GaladHen
{

    WindowGL::WindowGL(const char* name, unsigned int width, unsigned int height, bool maximizeWindow)
        : KeyboardCallback(nullptr)
        , MouseKeyCallback(nullptr)
        , MousePosCallback(nullptr)
        , KeyboardCallbackOwner(nullptr)
        , MouseKeyCallbackOwner(nullptr)
        , MousePosCallbackOwner(nullptr)
    {
        FillKeyAssociations();
        Init();

        if (maximizeWindow)
        {
            glm::uvec2 size = GetScreenSize();
            CreateOpenGLWindow(size.x, size.y, name);
            glfwMaximizeWindow(WinGL);
        }
        else
        {
            CreateOpenGLWindow(width, height, name);
        }
    }

    GLFWwindow* WindowGL::GetGLFWWindow()
    {
        return WinGL;
    }

    void WindowGL::FillKeyAssociations()
    {
        KeyboardKeyAssociations[GLFW_KEY_W] = (int)Input::KeyboardKey::W;
        KeyboardKeyAssociations[GLFW_KEY_A] = (int)Input::KeyboardKey::A;
        KeyboardKeyAssociations[GLFW_KEY_S] = (int)Input::KeyboardKey::S;
        KeyboardKeyAssociations[GLFW_KEY_D] = (int)Input::KeyboardKey::D;
        KeyboardKeyAssociations[GLFW_KEY_E] = (int)Input::KeyboardKey::E;
        KeyboardKeyAssociations[GLFW_KEY_Q] = (int)Input::KeyboardKey::Q;
        
        MouseKeyAssociations[GLFW_MOUSE_BUTTON_LEFT] = (int)Input::MouseKey::Left;
        MouseKeyAssociations[GLFW_MOUSE_BUTTON_RIGHT] = (int)Input::MouseKey::Right;

        KeyActionAssociations[GLFW_PRESS] = (int)Input::KeyAction::Pressed;
        KeyActionAssociations[GLFW_RELEASE] = (int)Input::KeyAction::Released;
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

    void WindowGL::Init()
    {
        // Init context

        if (!glfwInit())
        {
            Log::Error("RendererGL", "GLFW failed to initialize the context");

            return;
        }

        // setting the minimum required version of OpenGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLSL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLSL_VERSION_MINOR);
        // core profile is a subset of OpenGL features (without the backward-compatible features)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // for MacOS:
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }

    void WindowGL::CreateOpenGLWindow(unsigned int width, unsigned int height, const char* name)
    {

        WinGL = glfwCreateWindow(width, height, name, nullptr, nullptr);
        if (WinGL == nullptr)
        {
            Log::Error("WindowGL", "GLFW failed to create the window");

            return;
        }

        glfwMakeContextCurrent(WinGL);

        SetWindowSize(glm::uvec2(width, height));

        // After changes to GLFWwindow, we need to initialize gl3w context again
        //if (!gl3wInit())
        //{
        //    // TODO: Find out why gl3w fails creating the context, but without this gl3wInit() call the first call to a gl function will crash
        //    Log::Error("WindowGL", "Error: GL3W failed to initialize the context");
        //}
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

    glm::uvec2 WindowGL::GetScreenSize()
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        
        return glm::uvec2{ mode->width, mode->height };
    }

    glm::uvec2 WindowGL::GetWindowPosition()
    {
        int posX, posY;
        glfwGetWindowPos(WinGL, &posX, &posY);

        return glm::uvec2{ posX, posY };
    }

    glm::uvec2 WindowGL::GetWindowSize()
    {
        int width, height;
        glfwGetWindowSize(WinGL, &width, &height);

        return glm::uvec2{ width, height };
    }

    void WindowGL::SetWindowSize(glm::uvec2 size)
    {
        glfwSetWindowSize(WinGL, size.x, size.y);
        //glViewport(0, 0, width, height);
    }

    void WindowGL::CloseWindow()
    {
        glfwDestroyWindow(WinGL);
        glfwTerminate();
    }

    void WindowGL::SwapBuffers()
    {
        glfwSwapBuffers(WinGL);
    }

    WindowGL::~WindowGL()
    {
        // TODO
    }
}


