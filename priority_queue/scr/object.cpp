#include "object.h"
Object::Object(Texture2D _texture, glm::vec2 _position, glm::vec2 _scale, 
	glm::vec4 _color, int _value, bool _active) {
	position = _position;
	scale = _scale;
	color = _color;
	texture = _texture;
	value = _value;
	active = _active;
}
void Object::drawSelf(Renderer& renderer, glm::vec2 positionOfParent, int i) {
	glm::vec4 _color;
	if (active) _color = altcolor;
	else if (active2) _color = altcolor2;
	else _color = color;
	renderer.drawQuad(texture,position,scale,_color);
	renderer.drawLine(positionOfParent,position,color);
	renderer.drawText(std::to_string(value), { position.x,position.y }, 0.38f, _color);
}