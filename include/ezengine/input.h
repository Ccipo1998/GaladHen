
#pragma once

// glfw
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

class Input
{

public:

    /*
    @brief
    Callback functions link with specified glfw window
    @param window: glfw window to link for input callbacks
    */
    static void LinkInputCallbacks(GLFWwindow* window);

    /*
    @brief
    Perform the operations needed at each frame
    */
    static void Update();

    /*
    @brief
    Return a pointer to the array of keys status
    */
    static const bool* GetKeys();

    /*
    @brief
    Return a pointer to the array of mouse keys status
    */
    static const bool* GetMouseKeys();

    /*
    @brief
    Return current mouse x coordinate
    */
    static double GetMouseX();

    /*
    @brief
    Return current mouse y coordinate
    */
    static double GetMouseY();

    /*
    @brief
    Return delta mouse x coordinate between previous and current positions
    */
    static double GetDeltaMouseX();

    /*
    @brief
    Return delta mouse y coordinate between previous and current positions
    */
    static double GetDeltaMouseY();

private:

    // pointer to current linked glfw window
    static GLFWwindow* linkedWindow;

    // we initialize an array of booleans for each keybord key
    static bool keys[1024];

    // array for mouse buttons
    static bool mouse_keys[12];

    // mouse coordinates
    static double mouseX;
    static double mouseY;
    static double lastMouseX;
    static double lastMouseY;

    /*
    @brief
    Keys callbacks responses
    @param window: pointer to the GLFWwindow created when the context was initialized
    @param key: integer representing the activated key
    @param action: integer representing the action on the key
    */
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

    /*
    @brief
    Mouse keys callbacks responses
    @param window: pointer to the GLFWwindow created when the context was initialized
    @param button: integer representing the activated mouse key
    @param action: integer representing the action on the mouse key
    */
    static void MouseKeyCallback(GLFWwindow* window, int button, int action, int mods);

};
