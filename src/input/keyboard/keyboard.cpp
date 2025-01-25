#include "keyboard.h"
#include "../../core/editor.h"
#include <iostream>

void KeyboardHandler::CharCallback(GLFWwindow *window, unsigned int codepoint)
{
    Editor *handler = (Editor *)glfwGetWindowUserPointer(window);
    char latestChar[2] = {(char)codepoint,'\0'};
    handler->InsertText(latestChar);
}

// write a function to go to last char when moving vertically

void KeyboardHandler::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Editor *handler = (Editor *)glfwGetWindowUserPointer(window);

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        handler->SetCursorActive(true);
        if (key == GLFW_KEY_RIGHT)
        {
            handler->MoveCursor(Direction::right);
            // Move cursor right
            // Go to next line
        }
        if (key == GLFW_KEY_LEFT)
        {
            handler->MoveCursor(Direction::left);
            // Move cursor left
            // Go to prev line
        }
        if (key == GLFW_KEY_UP)
        {
            handler->MoveCursor(Direction::up);
            // Move one up if allowed, move to eol or same column equivalent
        }
        if (key == GLFW_KEY_DOWN)
        {
            handler->MoveCursor(Direction::down);
            // Move one down if allowed, move to eol or same column equivalent
        }
        if (key == GLFW_KEY_ENTER)
        {
            handler->InsertText("\n");
            // Insert new line character move appropriately
        }
        if (key == GLFW_KEY_BACKSPACE)
        {
            handler->DeleteText(1);
            // Delete from previous line if at first column and prev exists, move up and left
            // Delete from same line if text exists, move left
        }
    }
    else
    {
        handler->SetCursorActive(false);
    }
}
