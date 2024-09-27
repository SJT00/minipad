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

enum sourceType {
    original,
    add
};
struct Pieces
{
    unsigned int start;
    unsigned int length;
    const sourceType source;
};

struct PieceTable
{
    std::string originalBuffer;
    std::string add;
    Pieces piece[1];
};

class Editor
{ // and sickness continues
public:
    Editor(unsigned int width = Globals::scrWidth, unsigned int height = Globals::scrHeight);
    void SetCallbacks(GLFWwindow *window);
    // renders via grid to coordinate conversion with top-left being (0,0)
    void Render();
    // triggers cursor blinking state, true = non blinking
    void SetCursorActive(bool active);
    glm::vec2 GridToInt(glm::vec2 gridPos);
    std::vector<std::string> textArr;
    // PieceTable pieceTable;
    // cursor starts at grid pos 0,0
    glm::vec2 cursorloc;
    CursorRenderer cursorRenderer;
    TextRenderer textRenderer;
    int cols = (Globals::scrWidth - 2 * Globals::padding) / (Globals::fontWidth);

private:
    KeyboardHandler keyboardHandler;
    int rows = (Globals::scrHeight - Globals::padding) / (Globals::fontSize + Globals::lineSpacing);
};

#endif