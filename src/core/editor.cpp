#include "editor.h"

Editor::Editor(unsigned int width, unsigned int height, std::string fname)
    : cursorRenderer(width, height), textRenderer(width, height), pieceTable()
{
}

void Editor::SetCallbacks(GLFWwindow *window)
{
    glfwSetWindowUserPointer(window, this); // Pointer used for external manipulation of editor by keyboardHandler
    glfwSetCharCallback(window, keyboardHandler.CharCallback);
    glfwSetKeyCallback(window, keyboardHandler.KeyCallback);
}

void Editor::SetCursorActive(bool active)
{
    this->cursorRenderer.active = active;
}

void Editor::MoveCursor(Direction dir)
{
    switch (dir)
    {
    case Direction::right:
        break;
    case Direction::left:
        break;
    case Direction::up:
        break;
    case Direction::down:
        break;
    default:
        break;
    }
}

void Editor::Scroll(Direction dir)
{
    switch (dir)
    {
    case Direction::right:
        break;
    case Direction::left:
        break;
    case Direction::up:
        break;
    case Direction::down:
        break;
    default:
        break;
    }
}

void Editor::InsertText(const std::string &text)
{
    pieceTable.Insert(cursor.offset, text);
}

void Editor::DeleteText(unsigned int length)
{
    pieceTable.Delete(cursor.offset, length);
}

void Editor::Render()
{
    this->textRenderer.RenderText(this->pieceTable.getContent(), Globals::PADDING, Globals::VIEWPORT.TOP);
    this->cursorRenderer.RenderCursor(cursor.x, cursor.y);
}