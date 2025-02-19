#pragma once
#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <map>
#include "../../globals.h"
#include "../shader.h"

struct Cursor;
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
    TextRenderer(unsigned int width = Globals::SCR_WIDTH, unsigned int height = Globals::SCR_HEIGHT, Cursor *cursor = nullptr);
    // Gets and binds relevant font textures per char
    void Load(std::string fontSrc = Globals::FONTSRC, unsigned int fontSize = Globals::FONTSIZE);
    // Renders via precompiled characters map
    void RenderText(std::string text, float x, float y, float scale = 1.0f, glm::vec3 color = glm::vec3(1.0f));
    void TextRenderCall(int length);
    unsigned int scrollOffsetX = 0;
    unsigned int scrollOffsetY = 0;

private:
    std::map<GLchar, Character> Characters;
    GLuint textureArray;
    std::vector<glm::mat4> transforms;
    std::vector<int> letterMap;
    Shader TextShader;
    unsigned int fontSize;
    // Vtx arr obj and vtx buffer obj for render state
    unsigned int VAO, VBO;
    // Pointer to editors cursor obj used for rendering optimization
    Cursor *cursor;
};

#endif