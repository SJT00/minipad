#include "editor.h"

Editor::Editor(unsigned int width, unsigned int height, std::string filePath)
    : cursorRenderer(width, height), textRenderer(width, height, &cursor), pieceTable(filePath.length() > 0 ? LoadFile(filePath) : "")
{
}

std::string Editor::LoadFile(const std::string &filePath)
{
    std::ifstream infile{filePath};
    return {std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>()};
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
        cursor.offset = pieceTable.MoveCursorX(cursor.offset, 1);
        break;
    case Direction::left:
        cursor.offset = pieceTable.MoveCursorX(cursor.offset, -1);
        break;
    case Direction::up:
        cursor.offset = pieceTable.MoveCursorY(cursor.offset, 1);
        break;
    case Direction::down:
        cursor.offset = pieceTable.MoveCursorY(cursor.offset, -1);
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

void Editor::InsertText(const char *text)
{
    pieceTable.Insert(cursor.offset, text);
    contentChanged = true;
}

void Editor::DeleteText(unsigned int length)
{
    if (cursor.offset > 0)
    {
        pieceTable.Delete(cursor.offset, length);
        contentChanged = true;
    }
}

void Editor::Render()
{
    if (contentChanged)
    {
        cachedContent = this->pieceTable.GetContent(); // Update only if changed
        contentChanged = false;                        // Reset change flag
    }
    this->textRenderer.RenderText(cachedContent, Globals::VIEWPORT.LEFT, Globals::VIEWPORT.TOP);
    this->cursorRenderer.RenderCursor(cursor.x, cursor.y);
}

void Editor::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    this->textRenderer.RenderText(text, x, y, scale, color);
}