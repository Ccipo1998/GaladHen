
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
        , OutMouseKeyCallback(nullptr)
        , OutMousePosCallback(nullptr)
    {
        // TODO: add a static way to retrieve current api selection, this changes the way of win api creation
        WinAPI = new WindowGL(1280, 720, "");
    }

    Window::Window(const std::string& windowName, unsigned int width, unsigned int height)
        : WindowName(windowName)
        , Width(width)
        , Height(height)
        , OutKeyboardCallback(nullptr)
        , OutMouseKeyCallback(nullptr)
        , OutMousePosCallback(nullptr)
    {
        // TODO: add a static way to retrieve current api selection, this changes the way of win api creation
        WinAPI = new WindowGL(width, height, windowName.data());
    }

    float Window::GetAspectRatio()
    {
        return static_cast<float>(Width) / static_cast<float>(Height);
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

    Window::~Window()
    {
        // TODO
    }
}
