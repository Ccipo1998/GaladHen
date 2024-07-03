
// The integers representing keys, actions and similar are already been translated at api level to be correct inside this higher level class

#pragma once

namespace GaladHen
{
    enum class KeyboardKey
    {
        W = 0,
        A = 1,
        S = 2,
        D = 3,
        E = 4,
        Q = 5
    };

    enum class MouseKey
    {
        Left = 0,
        Right = 1
    };

    enum class KeyAction
    {
        Pressed = 0,
        Released = 1
    };
}

namespace GaladHen
{
    class Input
    {
    public:

        Input();

        // @brief
        // Keyboard callback response
        // @param sender: a generic pointer to the object invoking the callback
        // @param key: integer representing the activated key
        // @param action: the action type on the key
        void KeyboardCallback(void* sender, unsigned int key, unsigned int action);

        // @brief
        // Mouse key callback response
        // @param sender: a generic pointer to the object invoking the callback
        // @param button: integer representing the activated mouse key
        // @param action: the action type on the key
        void MouseKeyCallback(void* sender, unsigned int key, unsigned int action);

        // @brief
        // Mouse position callback response
        // @param sender: a generic pointer to the object invoking the callback
        // @param mouseX: new mouse x coordinate
        // @param mouseY: new mouse y coordinate
        void MousePositionCallback(void* sender, float mouseX, float mouseY);

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

    };
}
