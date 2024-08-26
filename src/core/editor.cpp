#include "editor.h"

Editor::Editor(unsigned int width, unsigned int height)
    : textRenderer(), cursorRenderer(), keyboardHandler(&textRenderer, &cursorRenderer)
{
    this->textRenderer.Load("./assets/fonts/Futura.ttf", 14);
}

void Editor::setCallbacks(GLFWwindow *window)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetCharCallback(window, keyboardHandler.CharCallback);
    glfwSetKeyCallback(window, keyboardHandler.KeyCallback);
}

void Editor::Render()
{
    this->textRenderer.RenderText(this->text, 10.0f, 584.0f);
    this->cursorRenderer.RenderCursor(this->cursorloc[0], this->cursorloc[1]);
}