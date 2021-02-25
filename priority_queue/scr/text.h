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
        unsigned int TextureID; // ID �������� �����
        glm::ivec2   Size;      // ������ �����
        glm::ivec2   Bearing;   // �������� �� ����� ������ �� ��������/������ ���� �����
        unsigned int Advance;   // �������� �� ���������� �����
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

