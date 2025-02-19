#include "text.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../shader.h"
#include "../../core/editor.h"

#include <iostream>

// Update VS and FS if you change this! or make it a UBO
const unsigned int ARRAY_LIMIT = 254; // Batch Size, OS specific maximum
unsigned int texSize = 48;

TextRenderer::TextRenderer(unsigned int width, unsigned int height, Cursor *cursor)
    : TextShader("./src/renderer/text/shaders/text_2d.vs", "./src/renderer/text/shaders/text_2d.fs"), cursor(cursor)
{
    this->TextShader.use();
    this->TextShader.setMat4("projection", glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f));

    this->Load();
}

void TextRenderer::Load(std::string fontSrc, unsigned int fontSize)
{
    this->Characters.clear();
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

    FT_Set_Pixel_Sizes(face, texSize, texSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &textureArray);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, texSize, texSize, ARRAY_LIMIT, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            printf("ERROR::FREETYTPE: Failed to load Glyph\n");
            continue;
        }

        glTexSubImage3D(
            GL_TEXTURE_2D_ARRAY,
            0, 0, 0, static_cast<GLint>(c),
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows, 1,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);

        Character character = {
            static_cast<unsigned int>(c),
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)};
        Characters.insert(std::pair<char, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    this->fontSize = fontSize;
    letterMap.resize(ARRAY_LIMIT, 0);
    transforms.resize(ARRAY_LIMIT, glm::mat4(1.0f));

    GLfloat vertex_data[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::RenderText(
    std::string text, float x, float y, float scale, glm::vec3 color)
{
    scale = scale * fontSize / texSize;
    float copyX = x;

    this->TextShader.use();
    this->TextShader.setVec3("textColor", color);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
    this->TextShader.setInt("text", 0);

    glBindVertexArray(VAO);

    unsigned int workingIndex = 0;
    unsigned int documentOffset = 0;
    for (char c : text)
    {
        Character ch = Characters[c];

        if (documentOffset == 0 && x < Globals::SCR_WIDTH / 2)
        {
            this->cursor->x = x;
            this->cursor->y = y;
        }

        if (c == '\n')
        {
            y += Globals::FONTSIZE + Globals::LINESPACING;
            x = copyX;
            if (documentOffset + 1 == this->cursor->offset)
            {
                this->cursor->x = x;
                this->cursor->y = y;
            }
        }
        else if (std::isspace(c))
        {
            x += (ch.Advance >> 6) * scale;
        }
        else
        {
            float xpos = x + ch.Bearing.x * scale;
            float ypos = y + (texSize - ch.Bearing.y) * scale - texSize / 4;
            float size = texSize * scale;

            transforms[workingIndex] = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, 0.0f)) *
                                       glm::scale(glm::mat4(1.0f), glm::vec3(size, size, 1.0f));
            letterMap[workingIndex] = ch.TextureID;

            x += (ch.Advance >> 6) * scale;
            workingIndex++;
            if (workingIndex == ARRAY_LIMIT)
            {
                TextRenderCall(workingIndex);
                workingIndex = 0;
            }
        }
        documentOffset++;
        if (documentOffset == this->cursor->offset)
        {
            this->cursor->x = x;
            this->cursor->y = y;
        }
    }

    TextRenderCall(workingIndex);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void TextRenderer::TextRenderCall(int length)
{
    if (length != 0)
    {
        this->TextShader.use();
        TextShader.setIntVec("letterMap", letterMap[0], length);
        TextShader.setMat4("transforms", transforms[0], length);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, length);
    }
}