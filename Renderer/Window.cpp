
#include "Window.h"
#include <Renderer/LayerAPI/IWindowAPI.h>
#include <Renderer/LayerAPI/OpenGL/WindowGL.h>

namespace GaladHen
{
    Window::Window(API apiToUse)
        : WindowName("")
        , Width(1280)
        , Height(720)
        , OutKeyboardCallback(nullptr)
        , OutMouseKeyCallback(nullptr)
        , OutMousePosCallback(nullptr)
        , OutClosingWindowCallback(nullptr)
        , OutKeyboardCallbackOwner(nullptr)
        , OutMouseKeyCallbackOwner(nullptr)
        , OutMousePosCallbackOwner(nullptr)
    {
        switch (apiToUse)
        {
        case API::OpenGL:

            // Create OpenGL window
            WinAPI = new WindowGL(1280, 720, "");

            break;

        default:
            break;
        }
    }

    Window::Window(API apiToUse, const std::string& windowName, unsigned int width, unsigned int height)
        : WindowName(windowName)
        , Width(width)
        , Height(height)
        , OutKeyboardCallback(nullptr)
        , OutMouseKeyCallback(nullptr)
        , OutMousePosCallback(nullptr)
        , OutClosingWindowCallback(nullptr)
        , OutKeyboardCallbackOwner(nullptr)
        , OutMouseKeyCallbackOwner(nullptr)
        , OutMousePosCallbackOwner(nullptr)
    {
        switch (apiToUse)
        {
        case API::OpenGL:

            // Create OpenGL window
            WinAPI = new WindowGL(width, height, windowName.data());

            break;

        default:
            break;
        }
    }

    float Window::GetAspectRatio()
    {
        return static_cast<float>(Width) / static_cast<float>(Height);
    }

    void Window::CloseWindow()
    {
        WinAPI->CloseWindow();
    }
    
    void Window::SetKeyboardCallback(void (Input::*callback)(void* sender, unsigned int key, unsigned int action), Input* owner)
    {
        OutKeyboardCallback = callback;
        OutKeyboardCallbackOwner = owner;
        WinAPI->RegisterKeyboardCallback((void (*)(void*, unsigned int, unsigned int))&Window::KeyboardCallback, this);
    }

    void Window::SetMouseKeyCallback(void (Input::*callback)(void* sender, unsigned int key, unsigned int action), Input* owner)
    {
        OutMouseKeyCallback = callback;
        OutMouseKeyCallbackOwner = owner;
        WinAPI->RegisterMouseKeyCallback((void (*)(void*, unsigned int, unsigned int))&Window::MouseKeyCallback, this);
    }

    void Window::SetMousePositionCallback(void (Input::* callback)(void* sender, float mouseX, float mouseY), Input* owner)
    {
        OutMousePosCallback = callback;
        OutMousePosCallbackOwner = owner;
        WinAPI->RegisterMousePositionCallback((void (*)(void*, float, float)) & Window::MousePosCallback, this);
    }

    void Window::SetClosingWindowCallback(void(Input::* callback)(void* sender), Input* owner)
    {
        OutClosingWindowCallback = callback;
        OutClosingWindowCallbackOwner = owner;
        WinAPI->RegisterClosingWindowCallback((void (*)(void*))&Window::ClosingWindowCallback, this);
    }

    void Window::CallKeyboardCallback(unsigned int key, unsigned int action)
    {
        (OutKeyboardCallbackOwner->*OutKeyboardCallback)(this, key, action);
    }

    void Window::CallMouseKeyCallback(unsigned int key, unsigned int action)
    {
        (OutMouseKeyCallbackOwner->*OutMouseKeyCallback)(this, key, action);
    }

    void Window::CallMousePositionCallback(float mouseX, float mouseY)
    {
        (OutMousePosCallbackOwner->*OutMousePosCallback)(this, mouseX, mouseY);
    }

    void Window::CallClosingWindowCallback()
    {
        (OutClosingWindowCallbackOwner->*OutClosingWindowCallback)(this);
    }

    void Window::KeyboardCallback(Window* owner, unsigned int key, unsigned int action)
    {
        owner->CallKeyboardCallback(key, action);
    }
    
    void Window::MouseKeyCallback(Window* owner, unsigned int key, unsigned int action)
    {
        owner->CallMouseKeyCallback(key, action);
    }

    void Window::MousePosCallback(Window* owner, float mouseX, float mouseY)
    {
        owner->CallMousePositionCallback(mouseX, mouseY);
    }

    void Window::ClosingWindowCallback(Window* owner)
    {
        owner->CallClosingWindowCallback();
    }

    void Window::BeginFrame()
    {
        WinAPI->ClearFrontBuffers(true, true, true); // TODO: clear parameters exposed in Window class?
    }

    void Window::EndFrame()
    {
        WinAPI->SwapBuffers();
        WinAPI->InvokePendingCallbacks();
    }

    void Window::SetColorBufferClearColor(glm::vec4 color)
    {
        WinAPI->SetColorBufferClearColor(color);
    }

    Window::~Window()
    {
        // TODO
    }
}
