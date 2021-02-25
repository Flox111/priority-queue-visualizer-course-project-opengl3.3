#pragma once
#include<glad/glad.h>
#include <glm/glm.hpp>
#include<map>
#include<string>
#include"shader.h"
class Text
{
private:
    struct Character {
        unsigned int TextureID; // ID текстуры глифа
        glm::ivec2   Size;      // размер глифа
        glm::ivec2   Bearing;   // смещение от линии шрифта до верхнего/левого угла глифа
        unsigned int Advance;   // смещение до следующего глифа
    };
    std::map<GLchar, Character> Characters;
    unsigned int VAO, VBO;
    int width, height;
    Shader textShader;
public:
    int init(int const _height, int const _width);
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
    float getSizeString(std::string text,float scale);
};

