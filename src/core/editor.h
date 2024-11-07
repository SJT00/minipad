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

enum sourceType
{
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

// Need to change structure of this class, who owns render responsibility, keyboard responsibility etc.
// Will look into it later

class Editor
{
public:
    Editor(unsigned int &width = Globals::scrWidth, unsigned int &height = Globals::scrHeight, const char *fname = "");
    // Loads/Create File on to memory
    void LoadFile(const char *fname);
    // OnClose Handler, cleanup ptrs and save
    void OnClose();
    // Set various Keypress and Mouse movement callbacks
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

private:
    FILE *fptr;
    KeyboardHandler keyboardHandler;
    int cols = (Globals::scrWidth - 2 * Globals::padding) / (Globals::fontWidth);
    int rows = (Globals::scrHeight - Globals::padding) / (Globals::fontSize + Globals::lineSpacing);
};

#endif