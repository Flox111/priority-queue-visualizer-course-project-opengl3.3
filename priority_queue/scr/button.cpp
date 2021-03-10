#include "button.h"
Button::Button(Texture2D _texture, glm::vec2 _position, glm::vec2 _scale, glm::vec4 _color,const char* _type, int _width, int _height) {
	position = _position;
	scale = _scale;
	color = _color;
	texture = _texture;
	type = _type;
	width - _width;
	height = _height;
}
void Button::drawSelf(Renderer& renderer) {
	renderer.drawQuad(texture, position, scale, active ? glm::vec4(0.1f, 0.47f, 0.29f, 1.0f) : color);
	if (type == "Label") {
		renderer.drawText(value, position, 0.45f, color);
	}
	else renderer.drawText(type, position, 0.45f, color);
}
void Button::pushButton() {
	this->active = !active;
}
bool Button::checkCollision(double& x, double& y) {
	if (x <= position.x + scale.x && x >= position.x - scale.x &&
		height - y <= position.y + scale.y && height - y >= position.y - scale.y)
		return 1;
	else return 0;
}