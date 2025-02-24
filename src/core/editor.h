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
    Editor(unsigned int width = Globals::SCR_WIDTH, unsigned int height = Globals::SCR_HEIGHT, std::string filePath = "");
    // Load and feed file into Piece Table
    std::string LoadFile(const std::string &filePath);
    // Set various Keypress and Mouse movement callbacks
    void SetCallbacks(GLFWwindow *window);
    // Main render method
    void Render();
    // Render for external use (fps counter)
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
    // Triggers cursor blinking state, true = non blinking
    void SetCursorActive(bool active);
    void MoveCursor(Direction dir);

    void InsertText(const char *text);
    void DeleteText(unsigned int length);

    void Scroll(Direction dir);

private:
    CursorRenderer cursorRenderer;
    TextRenderer textRenderer;
    KeyboardHandler keyboardHandler;
    PieceTable pieceTable;
    Cursor cursor = {.offset = 0, .x = Globals::VIEWPORT.LEFT, .y = Globals::VIEWPORT.TOP};
    std::string cachedContent;  // Stores last rendered docstring
    bool contentChanged = true; // Flag to track changes
};

#endif