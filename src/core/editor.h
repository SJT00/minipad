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
#include "./piecetable.h"

struct Cursor
{
    unsigned int offset;
    unsigned int x;
    unsigned int y;
};

enum Direction
{
    up,
    down,
    left,
    right
};

class Editor
{
public:
    Editor(unsigned int width = Globals::SCR_WIDTH, unsigned int height = Globals::SCR_HEIGHT, std::string fname = "");
    // Set various Keypress and Mouse movement callbacks
    void SetCallbacks(GLFWwindow *window);
    // Renders via grid to coordinate conversion with top-left being (0,0)
    void Render();
    // Triggers cursor blinking state, true = non blinking
    void SetCursorActive(bool active);
    void MoveCursor(Direction dir);

    void InsertText(const std::string &text);
    void DeleteText(unsigned int length);

    void Scroll(Direction dir);

private:
    FILE *fptr;
    CursorRenderer cursorRenderer;
    TextRenderer textRenderer;
    KeyboardHandler keyboardHandler;
    PieceTable pieceTable;
    Cursor cursor = {.offset = 0, .x = Globals::VIEWPORT.LEFT, .y = Globals::VIEWPORT.TOP};
    int cols = (Globals::VIEWPORT.WIDTH) / (Globals::FONTWIDTH);
    int rows = (Globals::VIEWPORT.HEIGHT) / (Globals::FONTSIZE + Globals::LINESPACING);
};

#endif