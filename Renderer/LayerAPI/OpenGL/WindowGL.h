
#pragma once

#include <Renderer/LayerAPI/IWindowAPI.h>

class GLFWwindow;

namespace GaladHen
{
    class WindowGL : public IWindowAPI
    {
        
    public:

        WindowGL();

        WindowGL(unsigned int width, unsigned int height, const char* name);

        WindowGL(const WindowGL& other) = delete; //disallow copy
        WindowGL& operator=(const WindowGL& other) = delete;

        // INPUT --------------------------------------------------------------------------------------------------------------

        virtual void FillKeyAssociations() override;

        virtual void RegisterKeyboardCallback(void (*callback)(void* owner, unsigned int key, unsigned int action), void* owner) override;

        virtual void RegisterMouseKeyCallback(void (*callback)(void* owner, unsigned int key, unsigned int action), void* owner) override;
        
        virtual void RegisterMousePositionCallback(void (*callback)(void* owner, float mouseX, float mouseY), void* owner) override;

        virtual void GetCursorPosition(float& cursorX, float& cursorY) override;

        virtual void InvokePendingCallbacks() override;

        // @brief
        // Call the keyboard callback on the owner at higher level
        void SendKeyboardCallback(unsigned int key, unsigned int action);

        // @brief
        // Call the mouse key callback on the owner at higher level
        void SendMouseKeyCallback(unsigned int key, unsigned int action);

        // @brief
        // Call the mouse position callback on the owner at higher level
        void SendMousePositionCallback(float mouseX, float mouseY);

        unsigned int TranslateKeyboardKey(int glSpecificKey);

        unsigned int TranslateMouseKey(int glSpecificKey);

        unsigned int TranslateKeyAction(int glSpecificAction);

        // OPENGL -------------------------------------------------------------------------------------------------------------------------

        virtual void ClearFrontBuffers(bool colorBuffer, bool depthBuffer, bool stencilBuffer) override;

        virtual void SwapBuffers() override;

#pragma endregion

        virtual ~WindowGL() override;

    protected:

        // INPUT --------------------------------------------------------------------------------------------------------

        static void KeyboardCallbackGL(GLFWwindow* window, int key, int scancode, int action, int mode);
        static void MouseKeyCallbackGL(GLFWwindow* window, int button, int action, int mods);
        static void MousePosCallbackGL(GLFWwindow* window, double xpos, double ypos);

        // OPENGL ----------------------------------------------------------------------------------------------------

        // @brief
        // Initialization of the OpenGL context (glfw & gl3w) and of the window
        void InitContext(unsigned int width, unsigned int height, const char* name);

        GLFWwindow* WinGL;
        void* KeyboardCallbackOwner;
        void (* KeyboardCallback)(void* owner, unsigned int key, unsigned int action);
        void* MouseKeyCallbackOwner;
        void (* MouseKeyCallback)(void* owner, unsigned int key, unsigned int action);
        void* MousePosCallbackOwner;
        void (*MousePosCallback)(void* owner, float mouseX, float mouseY);

    };
}
