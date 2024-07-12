
// This is a pure virtual class (interface) to define basic functionalities of a window at API level

#pragma once

#include <glm/glm.hpp>

namespace GaladHen
{
    class IWindowAPI
    {
    
    public:

        // INPUT -------------------------------------------------------------------------------------------------

        virtual void FillKeyAssociations() = 0;

        virtual void RegisterKeyboardCallback(void (*callback)(void* owner, unsigned int key, unsigned int action), void* owner) = 0;

        virtual void RegisterMouseKeyCallback(void (*callback)(void* owner, unsigned int key, unsigned int action), void* owner) = 0;

        virtual void RegisterMousePositionCallback(void (*callback)(void* owner, float mouseX, float mouseY), void* owner) = 0;

        virtual void RegisterClosingWindowCallback(void (*callback)(void* owner), void* owner) = 0;

        virtual void GetCursorPosition(float& cursorX, float& cursorY) = 0;

        virtual void InvokePendingCallbacks() = 0;

        // RENDERING ---------------------------------------------------------------------------------------------

        virtual void ClearFrontBuffers(bool colorBuffer, bool depthBuffer, bool stencilBuffer) = 0;

        virtual void SwapBuffers() = 0;

        virtual void SetColorBufferClearColor(glm::vec4 color) = 0;

        virtual void CloseWindow() = 0;

        virtual ~IWindowAPI() {}; // not pure virtual because of linking errors

    protected:

        int KeyboardKeyAssociations[1024];
        int MouseKeyAssociations[12];
        int KeyActionAssociations[2];

    };
}
