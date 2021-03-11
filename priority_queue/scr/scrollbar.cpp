#include "scrollbar.h"
ScrollBar::ScrollBar(Texture2D _texture, glm::vec2 _position, glm::vec2 _boxScale, glm::vec2 _lineScale, glm::vec4 _color, int _width, int _height) {
	position = _position;
	centerPos = _position;
	boxScale = _boxScale;
	lineScale = _lineScale;
	color = _color;
	texture = _texture;
	width - _width;
	height = _height;
	startPos = centerPos.x - lineScale.x;
	endPos = centerPos.x + lineScale.x;
	maxValue = 5.0f;
	distance = endPos - startPos;
	speedAnim = maxValue * (position.x - startPos) / distance;
	isMove = false;
}

bool ScrollBar::checkCollision(double& x, double& y) {
	if (x <= position.x + boxScale.x && x >= position.x - boxScale.x &&
		height - y <= position.y + boxScale.y && height - y >= position.y - boxScale.y)
		return 1;
	else return 0;
}

void ScrollBar::move(float x, float y) {
	if (x > startPos + boxScale.x && x < endPos - boxScale.x) {
		position.x = x;
		speedAnim = maxValue * (position.x - startPos) / distance;
	}
}
void ScrollBar::drawSelf(Renderer& renderer) {
	renderer.drawQuad(texture, position, boxScale, color);
	renderer.drawQuad(texture, centerPos, lineScale, color);
}