
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
        Released = 0,
        Pressed = 1
    };
}

namespace GaladHen
{
    struct Input
    {
        // array of booleans for each keybord key
        bool Keyboard[1024];

        // array for mouse buttons
        bool Mouse[12];

        // mouse coordinates
        float MouseX;
        float MouseY;
        float LastMouseX;
        float LastMouseY;

        bool CloseWindowRequested;
    };
}
