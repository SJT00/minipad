#pragma once
#ifndef CURSORRENDERER_H
#define CURSORRENDERER_H

#include <glm/glm.hpp>
#include "../../globals.h"
#include "../shader.h"

class CursorRenderer
{
public:
    Shader CursorShader;
    CursorRenderer(unsigned int width = Globals::SCR_WIDTH, unsigned int height= Globals::SCR_HEIGHT);
    void RenderCursor(float x, float y, glm::vec3 color = glm::vec3(1.0f));
    bool active = false;

private:
    unsigned int VAO, VBO;
};

#endif