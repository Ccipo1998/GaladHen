
#pragma once

namespace GaladHen
{
    enum KeyboardKey
    {
        W = 0,
        A = 1,
        S = 2,
        D = 3
    };

    enum MouseKey
    {
        Left = 0,
        Right = 1
    };

    enum KeyAction
    {
        Pressed = 0,
        Released = 1
    };
}

namespace GaladHen
{
    class Window;
    
    class Input
    {
    public:

        Input();

        // @brief
        // Callback functions link with specified glfw window
        // @param window: window to link for input callbacks
        void LinkInputCallbacks(Window* window);

        // @brief
        // Perform the operations needed at each frame
        // @param window: window from which the updated data are taken
        void Update(Window* window);

        // @brief
        // Return the status of the keyboard's key
        bool GetKeyboardKey(unsigned int key);

        // @brief
        // Return the status of the mouse's key
        bool GetMouseKey(unsigned int key);

        // @brief
        // Return current mouse x coordinate
        float GetMouseX();

        // @brief
        // Return current mouse y coordinate
        float GetMouseY();

        // @brief
        // Return delta mouse x coordinate between previous and current positions
        float GetDeltaMouseX();

        // @brief
        // Return delta mouse y coordinate between previous and current positions
        float GetDeltaMouseY();

    protected:

        // array of booleans for each keybord key
        bool Keyboard[1024];

        // array for mouse buttons
        bool Mouse[12];

        // mouse coordinates
        float MouseX;
        float MouseY;
        float LastMouseX;
        float LastMouseY;

        // @brief
        // Keyboard callback response
        // @param window: pointer to the window invoking the callbacks
        // @param key: integer representing the activated key
        // @param action: the action type on the key
        void KeyboardCallback(Window* window, unsigned int key, unsigned int action);

        // @brief
        // Mouse callback response
        // @param window: pointer to the window invoking the callbacks
        // @param button: integer representing the activated mouse key
        // @param action: the action type on the key
        void MouseCallback(Window* window, unsigned int key, unsigned int action);

    };
}
