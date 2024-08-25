#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include "../renderer/text/text.h"
// #include "../renderer/cursor/cursor.h"
#include "../input/keyboard/keyboard.h"

class Editor
{
public:
    Editor(unsigned int width, unsigned int height);
    void setCallbacks(GLFWwindow *window);
    void Render();

private:
    TextRenderer textRenderer;
    // CursorRenderer cursorRenderer;
    KeyboardHandler keyboardHandler;

    // glm::vec3 cursorLocation;
};

#endif