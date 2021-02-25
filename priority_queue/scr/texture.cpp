#include "texture.h"
#include<iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../ext/stb_image.h"

Texture2D::Texture2D() {
	this->width = 1;
	this->height = 1;
	this->mode_wrap_s = GL_REPEAT;
	this->mode_wrap_t = GL_REPEAT;
	this->mode_filter_min = GL_NEAREST;
	this->mode_filter_mag = GL_LINEAR;
	this->format_loaded = GL_RGBA;
	this->format_internal = GL_RGBA;

	glGenTextures(1, &this->id);
}
void Texture2D::loadFromFile(const char* texPath, bool generate_mipmaps) {
	stbi_set_flip_vertically_on_load(true);

	int _width, _height, _nrChannels;
	unsigned char* data = stbi_load(texPath, &_width, &_height, &_nrChannels, STBI_rgb_alpha);
	this->width = _width;
	this->height = _height;

	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->mode_wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->mode_wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->mode_filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->mode_filter_mag);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D,0,this->format_internal,this->width,this->height,0,this->format_loaded, GL_UNSIGNED_BYTE, data);
		if (generate_mipmaps) glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << "\n";
	}
	stbi_image_free(data);
}

void Texture2D::bind() {
	glBindTexture(GL_TEXTURE_2D,this->id);
}