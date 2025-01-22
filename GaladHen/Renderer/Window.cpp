
#include "Window.h"
#include <Renderer/LayerAPI/IWindowAPI.h>
#include <Renderer/LayerAPI/OpenGL/WindowGL.h>

namespace GaladHen
{
    Window::Window()
        : WindowName("")
        , Width(0)
        , Height(0)
        , CurrentAPI(API::OpenGL)
        , WindowInput(Input{})
    {}

    Window& Window::operator=(Window&& other) noexcept
    {
        WindowName = other.WindowName;
        Width = other.Width;
        Height = other.Height;
        CurrentAPI = other.CurrentAPI;
        WindowInput = other.WindowInput;

        WinAPI = other.WinAPI;

        // We need to transfer all the callbacks' function pointer to new location -> call RegisterInputCallbacks() again
        RegisterInputCallbacks();

        other.WinAPI = nullptr;

        return *this;
    }

    Window::Window(Window&& other) noexcept
    {
        WindowName = other.WindowName;
        Width = other.Width;
        Height = other.Height;
        CurrentAPI = other.CurrentAPI;
        WindowInput = other.WindowInput;

        WinAPI = other.WinAPI;

        // We need to transfer all the callbacks' function pointer to new location -> call RegisterInputCallbacks() again
        RegisterInputCallbacks();

        other.WinAPI = nullptr;
    }

    void Window::Init(API apiToUse)
    {
        switch (CurrentAPI)
        {
        case API::OpenGL:

            // Create OpenGL window
            WinAPI = new WindowGL{ "", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, true };

            // Because maximize window is true by default, we need to retrieve actual window size
            glm::uvec2 size = WinAPI->GetWindowSize();
            Width = size.x;
            Height = size.y;

            break;

        default:
            break;
        }

        RegisterInputCallbacks();
    }

    void Window::Init(API apiToUse, const std::string& windowName)
    {
        switch (CurrentAPI)
        {
        case API::OpenGL:

            // Create OpenGL window
            WinAPI = new WindowGL{ windowName.data(), DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, true };

            // Because maximize window is true by default, we need to retrieve actual window size
            glm::uvec2 size = WinAPI->GetWindowSize();
            Width = size.x;
            Height = size.y;

            break;

        default:
            break;
        }

        RegisterInputCallbacks();
    }

    void Window::Init(API apiToUse, const std::string& windowName, unsigned int width, unsigned int height)
    {
        switch (CurrentAPI)
        {
        case API::OpenGL:

            // Create OpenGL window
            WinAPI = new WindowGL(windowName.data(), width, height, false);

            break;

        default:
            break;
        }

        RegisterInputCallbacks();
    }

    float Window::GetAspectRatio()
    {
        return static_cast<float>(Width) / static_cast<float>(Height);
    }

    void Window::CloseWindow()
    {
        WinAPI->CloseWindow();
    }

    API Window::GetWindowAPI() const
    {
        return CurrentAPI;
    }

    IWindowAPI* Window::GetAPILevelWindow()
    {
        return WinAPI;
    }

    glm::uvec2 Window::GetWindowPosition()
    {
        return WinAPI->GetWindowPosition();
    }

    glm::uvec2 Window::GetWindowSize()
    {
        return WinAPI->GetWindowSize();
    }

    void Window::CallKeyboardCallback(Input::KeyboardKey key, Input::KeyAction action)
    {
        Window::KeyboardCallback(this, (int)key, (int)action);
    }

    void Window::CallMouseKeyCallback(Input::MouseKey key, Input::KeyAction action)
    {
        Window::MouseKeyCallback(this, (int)key, (int)action);
    }

    void Window::CallMousePositionCallback(float mouseX, float mouseY)
    {
        Window::MousePosCallback(this, mouseX, mouseY);
    }

    void Window::CallClosingWindowCallback()
    {
        Window::ClosingWindowCallback(this);
    }

    bool Window::IsKeyPressed(Input::KeyboardKey key)
    {
        return WindowInput.Keyboard[(int)key];
    }

    bool Window::IsKeyPressed(Input::MouseKey key)
    {
        return WindowInput.Mouse[(int)key];
    }

    void Window::RegisterInputCallbacks()
    {
        WinAPI->RegisterKeyboardCallback((void (*)(void*, unsigned int, unsigned int)) & Window::KeyboardCallback, this);
        WinAPI->RegisterMouseKeyCallback((void (*)(void*, unsigned int, unsigned int)) & Window::MouseKeyCallback, this);
        //WinAPI->RegisterMousePositionCallback((void (*)(void*, float, float)) & Window::MousePosCallback, this);
        WinAPI->RegisterClosingWindowCallback((void (*)(void*)) & Window::ClosingWindowCallback, this);
    }

    void Window::KeyboardCallback(Window* owner, unsigned int key, unsigned int action)
    {
        owner->WindowInput.Keyboard[key] = action;
        //owner->Page->SendKeyboardCallback(owner, key, action);
    }
    
    void Window::MouseKeyCallback(Window* owner, unsigned int key, unsigned int action)
    {
        owner->WindowInput.Mouse[key] = action;
        //owner->Page->SendMouseKeyCallback(owner, key, action);
    }

    void Window::MousePosCallback(Window* owner, float mouseX, float mouseY)
    {
        owner->WindowInput.MouseX = mouseX;
        owner->WindowInput.MouseY = mouseY;
    }

    void Window::ClosingWindowCallback(Window* owner)
    {
        owner->WindowInput.CloseWindowRequested = true;
    }

    void Window::BeginFrame()
    {
        // update mouse position
        float currentX, currentY;
        GetMousePosition(currentX, currentY);
        WindowInput.LastMouseX = WindowInput.MouseX;
        WindowInput.LastMouseY = WindowInput.MouseY;
        WindowInput.MouseX = currentX;
        WindowInput.MouseY = currentY;

        /*Page->NewFrame();
        Page->BuildPage();*/
    }

    void Window::EndFrame()
    {
        //Page->Draw();

        WinAPI->SwapBuffers();
        WinAPI->InvokePendingCallbacks();
    }

    void Window::GetMousePosition(float& mouseX, float& mouseY)
    {
        WinAPI->GetCursorPosition(mouseX, mouseY);
    }

    void Window::GetMousePositionDelta(float& deltaX, float& deltaY)
    {
        deltaX = WindowInput.MouseX - WindowInput.LastMouseX;
        deltaY = WindowInput.MouseY - WindowInput.LastMouseY;
    }

    bool Window::IsCloseWindowRequested()
    {
        return WindowInput.CloseWindowRequested;
    }

    Window::~Window()
    {
        // TODO
    }
}
