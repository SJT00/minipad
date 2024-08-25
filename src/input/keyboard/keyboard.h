#pragma once
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <GLFW/glfw3.h>

class KeyboardHandler
{
public:
    KeyboardHandler();
    static void CharCallback(GLFWwindow *window, unsigned int codepoint);
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};

#endif