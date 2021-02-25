#pragma once
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"shader.h"
#include"texture.h"
#include"text.h"
class Renderer
{
public:
	void initialize(const int width,const int height);
	void drawQuad(Texture2D& texture,glm::vec2 position,glm::vec2 scale,glm::vec4 color);
	void drawLine(glm::vec2 position1, glm::vec2 position2, glm::vec4 color);
	void update_render_dimensions(int width, int height);
	void drawText(std::string text_to_draw, glm::vec2 pos, float _scale, glm::vec4 color, bool isButton = true);

	void clear(glm::vec4 color);
	~Renderer();
private:
	unsigned int quadVAO;
	int render_width, render_height;
	Shader shader;
	Text* text;
};

