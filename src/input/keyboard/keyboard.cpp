#include "keyboard.h"
#include <iostream>

KeyboardHandler::KeyboardHandler()
{
}

void KeyboardHandler::CharCallback(GLFWwindow *window, unsigned int codepoint)
{
    std::cout << codepoint << "\n";
}