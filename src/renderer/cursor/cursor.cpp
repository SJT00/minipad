#include "cursor.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

CursorRenderer::CursorRenderer(unsigned int width, unsigned int height)
    : CursorShader("./src/renderer/cursor/shaders/cursor.vs", "./src/renderer/cursor/shaders/cursor.fs")
{
    this->CursorShader.use();
    this->CursorShader.setMat4("projection", glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f));

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 2, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CursorRenderer::RenderCursor(float x, float y, glm::vec3 color)
{
    this->CursorShader.use();
    float newAlpha = (sin(glfwGetTime() * 2.0f * M_PI) + 1.0f) / 2.0f;
    this->CursorShader.setVec3("curColor", color);
    if (!this->active)
    {
        this->CursorShader.setFloat("alpha", newAlpha);
    }
    else
    {
        this->CursorShader.setFloat("alpha", 1.0f);
    }
    glBindVertexArray(this->VAO);
    int h = Globals::fontSize / 2;
    float vertices[2][2] = {
        {x, y + h},
        {x, y - h}};

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_LINES, 0, 2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}