#pragma once
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "texture.h"
#include"renderer.h"
#include"window.h"
#include"tree.h"
class Object
{
public:
	glm::vec2 position;
	glm::vec2 scale;
	glm::vec4 color;
	glm::vec4 altcolor = glm::vec4(0.1f,0.47f,0.29f,1.0f);
	glm::vec4 altcolor2 = glm::vec4(0.49f, 0.06f, 0.07f, 1.0f);
	Texture2D texture;
	int value = 0;
	bool active = false;
	bool active2 = false;
	Object(Texture2D _texture, glm::vec2 _position, glm::vec2 _scale, glm::vec4 color, int _value,bool _active = false);
	void drawSelf(Renderer& renderer, glm::vec2 positionOfParent, int i);
private:
};

