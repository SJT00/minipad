#include "text.h"
#include <glad/glad.h>

TextRenderer::TextRenderer()
{
    // FreeType: initialize and load
    FT_Library ft;

    if (FT_Init_FreeType(&ft))
    {
        printf("ERROR::FREETYPE: Could not init FreeType Library\n");
    }

    FT_Face face;
    if (FT_New_Face(ft, "assets/fonts/Futura.ttf", 0, &face))
    {
        printf("ERROR::FREETYPE: Failed to load font\n");
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    loadCharacters(&face);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::loadCharacters(FT_Face *face)
{
    if (face == nullptr)
    {
        printf("ERROR: FT_Face pointer is null\n");
        return;
    }
    if (*face == nullptr)
    {
        printf("ERROR: FT_Face is null\n");
        return;
    }
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(*face, c, FT_LOAD_RENDER))
        {
            printf("ERROR::FREETYTPE: Failed to load Glyph\n");
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            (*face)->glyph->bitmap.width,
            (*face)->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            (*face)->glyph->bitmap.buffer);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2((*face)->glyph->bitmap.width, (*face)->glyph->bitmap.rows),
            glm::ivec2((*face)->glyph->bitmap_left, (*face)->glyph->bitmap_top),
            (unsigned int)(*face)->glyph->advance.x};
        Characters.insert(std::pair<char, Character>(c, character));
    }
}