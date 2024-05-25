
#include "Window.h"
#include <Renderer/LayerAPI/WindowGL.h>

namespace GaladHen
{
    
    Window::Window()
        : WindowName("")
        , Width(1280)
        , Height(720)
        , OutKeyboardCallback(nullptr)
        , OutMouseCallback(nullptr)
        , OutKeyboardCallbackOwner(nullptr)
        , OutMouseCallbackOwner(nullptr)
    {
        // TODO: add a static way to retrieve current api selection, this changes the way of win api creation
        WinAPI = new WindowGL(1280, 720, "");
    }

    Window::Window(const std::string& windowName, unsigned int width, unsigned int height)
        : WindowName(windowName)
        , Width(width)
        , Height(height)
        , OutKeyboardCallback(nullptr)
        , OutMouseCallback(nullptr)
        , OutKeyboardCallbackOwner(nullptr)
        , OutMouseCallbackOwner(nullptr)
    {
        // TODO: add a static way to retrieve current api selection, this changes the way of win api creation
        WinAPI = new WindowGL(width, height, windowName.data());
    }

    float Window::GetAspectRatio()
    {
        return static_cast<float>(Width) / static_cast<float>(Height);
    }
    
    void Window::SetKeyboardCallback(void (Input::*callback)(Window*, unsigned int key, unsigned int action), Input* obj)
    {
        OutKeyboardCallback = callback;
        OutKeyboardCallbackOwner = obj;
        WinAPI->RegisterKeyboardCallback(&Window::KeyboardCallback, this);
    }

    void Window::SetMouseCallback(void (Input::*callback)(Window*, unsigned int key, unsigned int action), Input* obj)
    {
        OutMouseCallback = callback;
        OutMouseCallbackOwner = obj;
        WinAPI->RegisterKeyboardCallback(&Window::MouseCallback, this);
    }

    void Window::KeyboardCallback(unsigned int key, unsigned int action)
    {
        (OutKeyboardCallbackOwner->*OutKeyboardCallback)(this, key, action);
    }
    
    void Window::MouseCallback(unsigned int key, unsigned int action)
    {
        (OutMouseCallbackOwner->*OutMouseCallback)(this, key, action);
    }

    Window::~Window()
    {
        // TODO
    }
}
