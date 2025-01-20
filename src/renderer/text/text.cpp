#include "text.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../shader.h"

#include <iostream>

TextRenderer::TextRenderer(unsigned int width, unsigned int height)
    : TextShader("./src/renderer/text/shaders/text_2d.vs", "./src/renderer/text/shaders/text_2d.fs")
{
    this->TextShader.use();
    this->TextShader.setMat4("projection", glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f));

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    this->Load();
}

void TextRenderer::Load(std::string fontSrc, unsigned int fontSize)
{
    this->Characters.clear();
    // FreeType: initialize and load
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        printf("ERROR::FREETYPE: Could not init FreeType Library\n");
    }

    FT_Face face;
    if (FT_New_Face(ft, fontSrc.c_str(), 0, &face))
    {
        printf("ERROR::FREETYPE: Failed to load font\n");
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    // disable byte-alignment restriction due to using red grayscale as optimization
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
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
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x};
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(
    std::string text, float x, float y, float scale, glm::vec3 color)
{
    this->TextShader.use();
    this->TextShader.setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);
    this->TextShader.setInt("text", 0);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        if (*c == '\n')
        {
            y += Globals::FONTSIZE + Globals::LINESPACING;
            x = Globals::PADDING;
            continue;
        }

        float xpos = x + ch.Bearing.x * scale + scrollOffsetX;
        float ypos = y + (ch.Size.y - ch.Bearing.y) * scale - scrollOffsetY;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // Outside Viewport
        // if (ypos < Globals::VIEWPORT.TOP || ypos > Globals::VIEWPORT.BOTTOM || xpos < Globals::VIEWPORT.LEFT || xpos > Globals::VIEWPORT.RIGHT)
        // {
        //     std::cout<<"ypos+h: "<<ypos<<" h: "<<h<<std::endl;
        //     std::cout<<"ypos-h: "<<ypos<<" h: "<<h<<std::endl;
        //     std::cout<<"xpos-w: "<<xpos<<" w: "<<w<<std::endl;
        //     std::cout<<"xpos+w: "<<xpos<<" w: "<<w<<std::endl;
        //     continue;
        // }

        float vertices[6][4] = {
            {xpos, ypos - h, 0.0f, 0.0f},
            {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos - h, 0.0f, 0.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos - h, 1.0f, 0.0f}};

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Advance cursor for next char
        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}