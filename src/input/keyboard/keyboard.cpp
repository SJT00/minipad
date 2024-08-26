#include "keyboard.h"
#include "../../core/editor.h"
#include <iostream>

KeyboardHandler::KeyboardHandler(TextRenderer *textRenderer, CursorRenderer *cursorRenderer)
    : textRenderer(textRenderer), cursorRenderer(cursorRenderer)
{
}

void KeyboardHandler::CharCallback(GLFWwindow *window, unsigned int codepoint)
{
    Editor *handler = (Editor *)glfwGetWindowUserPointer(window);
    handler->text += codepoint;
}

void KeyboardHandler::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Editor *handler = (Editor *)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_RIGHT)
    {
        handler->cursorloc[0] += 6;
    }
    if (key == GLFW_KEY_LEFT)
    {
        handler->cursorloc[0] -= 6;
    }
    if (key == GLFW_KEY_UP)
    {
        handler->cursorloc[1] += 6;
    }
    if (key == GLFW_KEY_DOWN)
    {
        handler->cursorloc[1] -= 6;
    }
}