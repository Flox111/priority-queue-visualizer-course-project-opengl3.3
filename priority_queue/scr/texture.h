#pragma once
#include<glad/glad.h>
class Texture2D
{
public:
	unsigned int id;
	int width, height;
	unsigned int mode_wrap_s;
	unsigned int mode_wrap_t;
	unsigned int mode_filter_min;
	unsigned int mode_filter_mag;
	unsigned int format_loaded;
	unsigned int format_internal;

	Texture2D();
	void loadFromFile(const char* texPath, bool generate_mipmaps);
	void bind();
	
};

