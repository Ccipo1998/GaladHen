
#include <ezengine/input.h>

// static members definitions
GLFWwindow* Input::linkedWindow = nullptr;
bool Input::keys[] = {};
bool Input::mouse_keys[] = {};
double Input::mouseX = 0.0;
double Input::mouseY = 0.0;
double Input::lastMouseX = 0.0;
double Input::lastMouseY = 0.0;

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // we keep trace of the pressed keys
    // with this method, we can manage 2 keys pressed at the same time:
    // many I/O managers often consider only 1 key pressed at the time (the first pressed, until it is released)
    // using a boolean array, we can then check and manage all the keys pressed at the same time
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

void Input::MouseKeyCallback(GLFWwindow* window, int button, int action, int mods)
{
    if(action == GLFW_PRESS)
        mouse_keys[button] = true;
    else if(action == GLFW_RELEASE)
        mouse_keys[button] = false;
}

void Input::LinkInputCallbacks(GLFWwindow* window)
{
    Input::linkedWindow = window;

    // callbacks link
    glfwSetKeyCallback(window, Input::KeyCallback);
    glfwSetMouseButtonCallback(window, Input::MouseKeyCallback);
}

void Input::Update()
{
    // update mouse position
    Input::lastMouseX = Input::mouseX;
    Input::lastMouseY = Input::mouseY;
    glfwGetCursorPos(Input::linkedWindow, &mouseX, &mouseY);
}

const bool* Input::GetKeys()
{
    return Input::keys;
}

const bool* Input::GetMouseKeys()
{
    return Input::mouse_keys;
}

double Input::GetMouseX()
{
    return Input::mouseX;
}

double Input::GetMouseY()
{
    return Input::mouseY;
}

double Input::GetDeltaMouseX()
{
    return Input::mouseX - Input::lastMouseX;
}

double Input::GetDeltaMouseY()
{
    return Input::mouseY - Input::lastMouseY;
}
