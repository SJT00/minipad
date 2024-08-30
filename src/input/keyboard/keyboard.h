#pragma once
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../../renderer/text/text.h"
#include "../../renderer/cursor/cursor.h"
#include <GLFW/glfw3.h>

class KeyboardHandler
{
public:
    static void CharCallback(GLFWwindow *window, unsigned int codepoint);
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};

#endif