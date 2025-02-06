#include "editor.h"

Editor::Editor(unsigned int width, unsigned int height, string filePath)
    : cursorRenderer(width, height), textRenderer(width, height, &cursor), pieceTable(filePath.length() > 0 ? LoadFile(filePath) : "")
{
}

string Editor::LoadFile(const string &filePath)
{
    ifstream infile{filePath};
    return {istreambuf_iterator<char>(infile), istreambuf_iterator<char>()};
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
        if (cursor.offset < pieceTable.documentLength)
        {
            cursor.offset++;
        }
        break;
    case Direction::left:
        if (cursor.offset > 0)
        {
            cursor.offset--;
        }
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

void Editor::InsertText(const char *text)
{
    cout << "Cursor.offset: " << cursor.offset << endl;
    pieceTable.Insert(cursor.offset, text);
}

void Editor::DeleteText(unsigned int length)
{
    if (cursor.offset > 0)
    {
        pieceTable.Delete(cursor.offset, length);
    }
}

void Editor::Render()
{
    this->textRenderer.RenderText(this->pieceTable.GetContent(), Globals::PADDING, Globals::VIEWPORT.TOP);
    this->cursorRenderer.RenderCursor(cursor.x, cursor.y);
}

void Editor::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    this->textRenderer.RenderText(text, x, y, scale, color);
}