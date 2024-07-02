
#include "Window.h"
#include <Renderer/LayerAPI/IWindowAPI.h>
#include <Renderer/LayerAPI/OpenGL/WindowGL.h>

namespace GaladHen
{
    
    Window::Window()
        : WindowName("")
        , Width(1280)
        , Height(720)
        , OutKeyboardCallback(nullptr)
        , OutMouseCallback(nullptr)
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
    {
        // TODO: add a static way to retrieve current api selection, this changes the way of win api creation
        WinAPI = new WindowGL(width, height, windowName.data());
    }

    float Window::GetAspectRatio()
    {
        return static_cast<float>(Width) / static_cast<float>(Height);
    }
    
    void Window::SetKeyboardCallback(void (Input::*callback)(Window* sender, unsigned int key, unsigned int action), Input* owner)
    {
        OutKeyboardCallback = callback;
        OutKeyboardCallbackOwner = owner;
        WinAPI->RegisterKeyboardCallback((void (*)(void*, unsigned int, unsigned int))&Window::KeyboardCallback, this);
    }

    void Window::SetMouseCallback(void (Input::*callback)(Window* sender, unsigned int key, unsigned int action), Input* owner)
    {
        OutMouseCallback = callback;
        OutMouseCallbackOwner = owner;
        WinAPI->RegisterMouseCallback((void (*)(void*, unsigned int, unsigned int))&Window::MouseCallback, this);
    }

    void Window::CallKeyboardCallback(unsigned int key, unsigned int action)
    {
        (OutKeyboardCallbackOwner->*OutKeyboardCallback)(this, key, action);
    }

    void Window::CallMouseCallback(unsigned int key, unsigned int action)
    {
        (OutMouseCallbackOwner->*OutMouseCallback)(this, key, action);
    }

    void Window::KeyboardCallback(Window* owner, unsigned int key, unsigned int action)
    {
        owner->CallKeyboardCallback(key, action);
    }
    
    void Window::MouseCallback(Window* owner, unsigned int key, unsigned int action)
    {
        owner->CallMouseCallback(key, action);
    }

    void Window::GetCurrentMousePosition(float& mouseX, float& mouseY)
    {
        WinAPI->GetCursorPosition(mouseX, mouseY);
    }

    Window::~Window()
    {
        // TODO
    }
}
