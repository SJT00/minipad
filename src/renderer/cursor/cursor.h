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
    CursorRenderer(unsigned int width = Globals::scrWidth, unsigned int height= Globals::scrHeight);
    void RenderCursor(float x, float y, glm::vec3 color = glm::vec3(1.0f));

private:
    unsigned int VAO, VBO;
};

#endif