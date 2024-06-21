
// This is a pure virtual class (interface) to define basic functionalities of a window at API level

#pragma once

namespace GaladHen
{
    class IWindowAPI
    {
    
    public:

        IWindowAPI()
        {
            // Key associations created at ctor time
            FillKeyAssociations();
        }

        virtual void FillKeyAssociations() = 0;

        virtual void RegisterKeyboardCallback(void (*callback)(void* owner, unsigned int key, unsigned int action), void* owner) = 0;

        virtual void RegisterMouseCallback(void (*callback)(void* owner, unsigned int key, unsigned int action), void* owner) = 0;

        virtual void GetCursorPosition(float& cursorX, float& cursorY) = 0;

        virtual ~IWindowAPI() = 0;

    protected:

        int KeyboardKeyAssociations[1024];
        int MouseKeyAssociations[12];
        int KeyActionAssociations[2];

    };
}
