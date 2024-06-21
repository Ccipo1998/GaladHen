
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

        virtual void FillKeyAssociations() override;

        virtual void RegisterKeyboardCallback(void (*callback)(void* owner, unsigned int key, unsigned int action), void* owner) override;

        virtual void RegisterMouseCallback(void (*callback)(void* owner, unsigned int key, unsigned int action), void* owner) override;

        virtual void GetCursorPosition(float& cursorX, float& cursorY) override;

        // @brief
        // Call the keyboard callback on the owner at higher level
        void SendKeyboardCallback(unsigned int key, unsigned int action);

        // @brief
        // Call the mouse callback on the owner at higher level
        void SendMouseCallback(unsigned int key, unsigned int action);

        unsigned int TranslateKeyboardKey(int glSpecificKey);

        unsigned int TranslateMouseKey(int glSpecificKey);

        unsigned int TranslateKeyAction(int glSpecificAction);

        virtual ~WindowGL() override;

    protected:

        static void KeyboardCallbackGL(GLFWwindow* window, int key, int scancode, int action, int mode);
        static void MouseCallbackGL(GLFWwindow* window, int button, int action, int mods);

        // @brief
        // Initialization of the OpenGL context (glfw & gl3w) and of the window
        void InitContext(unsigned int width, unsigned int height, const char* name);

        GLFWwindow* WinGL;
        void* KeyboardCallbackOwner;
        void (* KeyboardCallback)(void* owner, unsigned int key, unsigned int action);
        void* MouseCallbackOwner;
        void (* MouseCallback)(void* owner, unsigned int key, unsigned int action);

    };
}
