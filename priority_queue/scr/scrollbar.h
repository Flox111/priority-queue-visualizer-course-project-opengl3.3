#pragma once
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "texture.h"
#include "renderer.h"
#include<string>
class ScrollBar
{
private:
	float startPos;
	float endPos;
	glm::vec4 color;
	Texture2D texture;
	glm::vec2 position;
	glm::vec2 boxScale;
	glm::vec2 lineScale;
	int width, height;
	glm::vec2 centerPos;
	float maxValue;
	float distance;
public:
	float speedAnim;
	bool isMove;
	ScrollBar(Texture2D _texture, glm::vec2 _position, glm::vec2 _boxScale, glm::vec2 _lineScale, glm::vec4 _color, int _width, int _height);
	ScrollBar() {	}
	bool checkCollision(double& x, double& y);
	void drawSelf(Renderer& renderer);
	void move(float x, float y);
};

