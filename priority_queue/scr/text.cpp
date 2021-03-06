#include "text.h"
#include<iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
int Text::init(int const _width, int const _height) {
	width = _width;
	height = _height;

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ����������� � ������������� ������
    Shader shader("Resources/Shaders/text_vertex.glsl", "Resources/Shaders/text_fragment.glsl");
    textShader = shader;
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // ���������� FreeType
    FT_Library ft;

    // ������ ���, ����� ��������� ������, ������� ����� ���������� �������� �� ���� ��������
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    // ���� � ����� ������
    std::string font_name = "Resources/Fonts/BetinaScriptExtraCTT.TTF";
    if (font_name.empty())
    {
        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        return -1;
    }

    // ��������� ����� � face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    else {
        // ������ ������ ��� �������� ������
        FT_Set_Pixel_Sizes(face, 0, 48);

        // ��������� ����������� ������������ ������
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // ��������� ����� �������� 
        for (unsigned int c = 0; c < 256; c++)
        {
            // ��������� ���� ������� 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }

            // ���������� ��������
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
                face->glyph->bitmap.buffer
            );

            // ������ ��� �������� ����������� �����
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // ������ ��������� ������ ��� ������������ �������������
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // ����������� �������������� �������
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // ������������� VAO/VBO ��� ���������� ���������������
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Text::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    // ���������� ��������������� ��������� ����������	
    textShader.use();
    glUniform3f(glGetUniformLocation(textShader.id, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // ���������� ��� �������
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // ��������� VBO ��� ������� �������
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        // �������� �������� ����� �� �������������
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // ��������� ���������� ������ VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // ����������� ����������� glBufferSubData, � �� glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // ������ ��������������
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // ������ ���������� �������� ��� ����������� ���������� ����� (�������� ��������, ��� ������ �������� ���������� � ��������, ������������ 1/64 �������)
        x += (ch.Advance >> 6) * scale; // ������� ����� �� 6 ��������, ����� �������� �������� � �������� (2^6 = 64 (��������� ���������� 1/64-� �������� �� 64, ����� �������� ���������� ��������))
    }
    //std::cout << x << std::endl;
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
float Text::getSizeString(std::string text, float scale) {
    std::string::const_iterator c;
    float w = 0;
    for (c = text.begin(); c != text.end(); c++){
        Character ch = Characters[*c];

        w += ch.Size.x * scale;
    }
    return w;
}
