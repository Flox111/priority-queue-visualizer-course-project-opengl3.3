#include "renderer.h"
#include"assets.h"
#include<iostream>
void Renderer::initialize(const int width, const int height) {
	this->render_width = width;
	this->render_height = height;
	this->shader = Assets::shaders["quad_shader"];
	float quad_vertices[] = {
		  -1.0f, -1.0f,      0.0f, 0.0f,
		   1.0f, -1.0f,      1.0f, 0.0f,
		   1.0f,  1.0f,      1.0f, 1.0f,

		  -1.0f, -1.0f,      0.0f, 0.0f,
		   1.0f,  1.0f,      1.0f, 1.0f,
		  -1.0f,  1.0f,      0.0f, 1.0f
	};
	unsigned int quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);

	glEnable(GL_CULL_FACE);

	// Blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Depth mask.
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	glViewport(0, 0, width, height);

	text = new Text();
	text->init(width, height);

	update_render_dimensions(width,height);
}
void Renderer::drawQuad(Texture2D& texture,glm::vec2 position,glm::vec2 scale,glm::vec4 color) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 projection = glm::ortho<float>(0.0f, this->render_width, 0.0f, this->render_height, -1.0f, 1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model,glm::vec3(position.x,position.y,0.0f));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));

	shader.use();
	shader.setMat4("model",model);
	shader.setMat4("projection", projection);
	shader.setVec4("color", color);

	glActiveTexture(GL_TEXTURE0);
	texture.bind();
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES,0,6);
}
void Renderer::drawLine(glm::vec2 position1, glm::vec2 position2, glm::vec4 color) {
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 projection = glm::ortho<float>(0.0f, this->render_width, 0.0f, this->render_height, -1.0f, 1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	float a = position2.y - position1.y;
	float b = position2.x - position1.x;
	float c = sqrt(a*a+b*b);
	glm::vec2 position = glm::vec2((position1.x + position2.x) / 2, (position1.y + position2.y) / 2);
	float angle = atan((position2.y- position1.y)/(position2.x - position1.x));
	int turn = angle > 0 ? 1 : -1;
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
	model = glm::rotate(model, turn == 1?angle:angle-3.14f, glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, glm::vec3(c/4, 8.0f, 1.0f));

	shader.use();
	shader.setMat4("model", model);
	shader.setMat4("projection", projection);
	shader.setVec4("color", color);

	glActiveTexture(GL_TEXTURE0);
	Assets::textures["arrow"].bind();
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void Renderer::drawText(std::string text_to_draw, glm::vec2 pos,float _scale, glm::vec4 color, bool isButton)
{
	text->RenderText(text_to_draw, isButton ? pos.x - text->getSizeString(text_to_draw, _scale) / 2.0f : pos.x, isButton ? pos.y - _scale * 4 : pos.y, _scale, color);
}
void Renderer::update_render_dimensions(int width, int height){
	glViewport(0, 0, width, height);

	this->render_width = width;
	this->render_height = height;

	glm::mat4 projection = glm::ortho<float>(0.0f, width, 0.0f, height, -1.0f, 1.0f);
	this->shader.use();
	this->shader.setMat4("projection", projection);
}
void Renderer::clear(glm::vec4 c) {
	glClearDepth(1.0f);
	glClearColor(c.x,c.y,c.z,c.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Renderer::~Renderer() {
	glDeleteVertexArrays(1, &this->quadVAO);
	delete text;
}