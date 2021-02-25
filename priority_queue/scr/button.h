#pragma once
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "texture.h"
#include "renderer.h"
#include<string>
class Button
{
public:
	glm::vec2 position;
	glm::vec2 scale;
	glm::vec4 color;
	Texture2D texture;
	const char* type;
	bool active = false;
	bool wasPress = false;
	int width, height;
	std::string value;
	Button(Texture2D _texture, glm::vec2 _position, glm::vec2 _scale, glm::vec4 color, const char* _type,int _width,int _height);
	void pushButton();
	bool checkCollision(double& x,double& y);
	void drawSelf(Renderer& renderer);
};

