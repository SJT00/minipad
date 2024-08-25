#pragma once
#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <map>
#include "../shader.h"

struct Character
{
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size;        // Size of glyph
    glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Offset to advance to next glyph
};

class TextRenderer
{
public:
    std::map<char, Character> Characters;
    Shader TextShader;
    TextRenderer(unsigned int width, unsigned int height);
    // gets and binds relevant font textures per char
    void Load(std::string font, unsigned int fontSize);
    // renders via precompiled characters map
    void RenderText(std::string text, float x, float y, float scale = 1.0f, glm::vec3 color = glm::vec3(1.0f));

private:
    // vtx arr obj and vtx buffer obj for render state
    unsigned int VAO, VBO;
};

#endif