#include "editor.h"

Editor::Editor(unsigned int width, unsigned int height)
    : cursorRenderer(), textRenderer()
{
    this->textRenderer.Load("./assets/fonts/Futura.ttf", Globals::fontSize);
    this->textArr.resize(static_cast<size_t>(this->rows));
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

glm::vec2 Editor::GridToInt(glm::vec2 gridPos)
{
    // set to top left col/row
    glm::vec2 returnVec = glm::vec2(Globals::padding, Globals::padding);
    if (gridPos[0] > this->cols)
    {
        returnVec[0] = Globals::padding;
        returnVec[1] += 1;
        return returnVec;
    }
    if (gridPos[1] > this->rows)
    {
        throw std::runtime_error("Rows out of bound");
    }

    unsigned int adv = 0;
    for (int chars = 0; chars < gridPos[0]; chars++)
    {
        char c = this->textArr[gridPos[1]][chars];
        Character cChar = this->textRenderer.Characters[c];
        adv += cChar.Advance >> 6;
    }
    returnVec[0] = Globals::padding + adv;
    returnVec[1] = Globals::padding + gridPos[1] * (Globals::fontSize + Globals::lineSpacing);
    return returnVec;
}

void Editor::Render()
{
    glm::vec2 cursorVec = GridToInt(this->cursorloc);
    int lastRow = 0;
    if (textArr[0].size() > 0)
    {
        while (textArr[lastRow].size() != 0 && lastRow < rows)
        {
            std::string currentRow = textArr[cursorloc[1]];
            if (currentRow.size() > cols)
            {
                textArr[cursorloc[1] + 1].insert(0, currentRow.substr(cols));
                textArr[cursorloc[1]].erase(cols);
                // textArr.insert(textArr.begin() + cursorloc[1] + 1, nLine);

                if (cursorloc[0] > cols)
                {
                    cursorloc[0] = 0;
                    cursorloc[1] += 1;
                }
            }
            lastRow += 1;
        }
    }
    for (int r = 0; r <= lastRow; r++)
    {
        glm::vec2 textVec = GridToInt(glm::vec2(0, r));
        this->textRenderer.RenderText(this->textArr[r], textVec[0], textVec[1]);
    }
    this->cursorRenderer.RenderCursor(cursorVec[0], cursorVec[1]);
}