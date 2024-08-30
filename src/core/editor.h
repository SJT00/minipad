#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include <iostream>
#include <vector>
#include <string>
#include "../globals.h"
#include "../renderer/text/text.h"
#include "../renderer/cursor/cursor.h"
#include "../input/keyboard/keyboard.h"

class Editor
{
public:
    Editor(unsigned int width = Globals::scrWidth, unsigned int height = Globals::scrHeight);
    void SetCallbacks(GLFWwindow *window);
    void SetCursorActive(bool active);
    void Render();
    glm::vec2 GridToInt(glm::vec2 gridPos);
    std::vector<std::string> textArr;
    glm::vec2 cursorloc = glm::vec2(0, 0);
    CursorRenderer cursorRenderer;
    TextRenderer textRenderer;

private:
    KeyboardHandler keyboardHandler;
    int rows = (Globals::scrHeight - Globals::padding) / (Globals::fontSize + Globals::lineSpacing);
    int cols = (Globals::scrWidth - Globals::padding) / (Globals::fontWidth);
};

#endif