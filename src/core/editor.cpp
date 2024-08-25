#include "editor.h"

Editor::Editor(unsigned int width, unsigned int height)
    : textRenderer(width, height)
{
    this->textRenderer.Load("./assets/fonts/Futura.ttf", 14);
}

void Editor::setCallbacks(GLFWwindow *window)
{
    glfwSetCharCallback(window, keyboardHandler.CharCallback);
}

void Editor::Render()
{
    this->textRenderer.RenderText("curr", 10.0f, 584.0f, 1.0f, glm::vec3(1, 1, 1));
}