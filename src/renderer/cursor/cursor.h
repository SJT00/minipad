#pragma once
#ifndef CURSORRENDERER_H
#define CURSORRENDERER_H

#include <glm/glm.hpp>
#include "../shader.h"

class CursorRenderer
{
public:
    Shader CursorShader;
    CursorRenderer();
    void RenderCursor(glm::vec3 cursorloc);

private:
};

#endif