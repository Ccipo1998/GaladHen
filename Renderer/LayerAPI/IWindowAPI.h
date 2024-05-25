
// This is a pure virtual class (interface) to define basic functionalities of a window at API level

#pragma once

//#include <GaladHen/Window.h>

class Window;

namespace GaladHen
{
    class IWindowAPI
    {
    
    public:

        virtual void RegisterKeyboardCallback(void (Window::*)(unsigned int key, unsigned int action), Window* obj) = 0;

        virtual void RegisterMouseCallback(void (Window::*)(unsigned int key, unsigned int action), Window* obj) = 0;

        virtual void GetCursorPosition(float& cursorX, float& cursorY) = 0;

        virtual ~IWindowAPI() = 0;

    };
}
