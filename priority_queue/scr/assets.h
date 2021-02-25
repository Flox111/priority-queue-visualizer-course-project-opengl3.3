#pragma once
#include<map>
#include"shader.h"
#include"texture.h"
class Assets
{
public:
	static std::map<const char*, Shader> shaders;
	static std::map<const char*, Texture2D> textures;

	static Shader get_shader(const char* name);

	static Texture2D get_texture(const char* name);

	static void dispose();

private:
	Assets() { }
};

