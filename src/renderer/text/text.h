#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <map>

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
    TextRenderer();

private:
    void loadCharacters(FT_Face *face);
};

#endif