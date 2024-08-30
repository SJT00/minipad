#include "editor.h"

Editor::Editor(unsigned int width, unsigned int height)
    : cursorRenderer(), textRenderer()
{
    this->textRenderer.Load("./assets/fonts/Futura.ttf", Globals::fontSize);
    this->textArr.resize(static_cast<size_t>(this->rows));
    std::cout << textArr.size() << "\n";
}

void Editor::SetCallbacks(GLFWwindow *window)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetCharCallback(window, keyboardHandler.CharCallback);
    glfwSetKeyCallback(window, keyboardHandler.KeyCallback);
}

void Editor::SetCursorActive(bool active)
{
    this->cursorRenderer.active = active;
}

void Editor::Render()
{
    this->textRenderer.RenderText(this->textArr[0], Globals::padding, 584.0f);
    // each typable row
    for (int r = 0; r < this->rows; r++)
    {
        this->cursorRenderer.RenderCursor(this->cursorloc[0], this->cursorloc[1] - r * (Globals::fontSize + Globals::lineSpacing));
    }
}