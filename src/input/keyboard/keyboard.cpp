#include "keyboard.h"
#include <iostream>

KeyboardHandler::KeyboardHandler()
{
}

void KeyboardHandler::CharCallback(GLFWwindow *window, unsigned int codepoint)
{
    // std::cout << codepoint << "\n";
}

void KeyboardHandler::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        std::cout << key << "\n";
    }
}