#include "keyboard.h"
#include "../../core/editor.h"
#include <iostream>

void KeyboardHandler::CharCallback(GLFWwindow *window, unsigned int codepoint)
{
    Editor *handler = (Editor *)glfwGetWindowUserPointer(window);
    char latestChar = (char)codepoint;
    handler->textArr[handler->cursorloc[1]].insert(handler->cursorloc[0], 1, latestChar);
    handler->cursorloc[0] += 1;
}

void KeyboardHandler::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Editor *handler = (Editor *)glfwGetWindowUserPointer(window);

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        handler->SetCursorActive(true);
        if (key == GLFW_KEY_RIGHT && handler->cursorloc[0] < handler->textArr[handler->cursorloc[1]].size())
        {
            handler->cursorloc[0] += 1;
        }
        if (key == GLFW_KEY_LEFT && handler->cursorloc[0] > 0)
        {
            handler->cursorloc[0] -= 1;
        }
        if (key == GLFW_KEY_UP && handler->cursorloc[1] > 0)
        {
            handler->cursorloc[1] -= 1;
        }
        if (key == GLFW_KEY_DOWN && handler->textArr[handler->cursorloc[1] + 1].size() > 0)
        {
            handler->cursorloc[1] += 1;
        }
        if (key == GLFW_KEY_ENTER)
        {
            handler->textArr[handler->cursorloc[1]].append("\n");
            handler->cursorloc[0] = 0;
            handler->cursorloc[1] += 1;
        }
        if (key == GLFW_KEY_BACKSPACE && handler->cursorloc[0] > 0)
        {
            handler->textArr[handler->cursorloc[1]].erase(handler->cursorloc[0] - 1, 1);
            handler->cursorloc[0] -= 1;
        }
    }
    else
    {
        handler->SetCursorActive(false);
    }
}