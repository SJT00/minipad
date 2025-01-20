#include "editor.h"

Editor::Editor(unsigned int width, unsigned int height, std::string fname)
    : cursorRenderer(width, height), textRenderer(width, height), pieceTable(fname)
{
}

void Editor::SetCallbacks(GLFWwindow *window)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetCharCallback(window, keyboardHandler.CharCallback);
    glfwSetKeyCallback(window, keyboardHandler.KeyCallback);
}

void Editor::SetCursorActive(bool active)
{
    this->cursor.active = active;
}

void Editor::ScrollX(unsigned int dir)
{
    this->textRenderer.scrollOffsetX += dir;
}

void Editor::ScrollY(unsigned int dir)
{
    this->textRenderer.scrollOffsetY += dir;
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
    this->textRenderer.RenderText("asmdklasmdklsamdklmaslkmd", Globals::PADDING, Globals::VIEWPORT.TOP);
    this->cursorRenderer.RenderCursor(cursor.x, cursor.y);
}