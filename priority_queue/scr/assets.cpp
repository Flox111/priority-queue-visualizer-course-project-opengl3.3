#include "assets.h"
#include<glad/glad.h>
std::map<const char*, Shader>       Assets::shaders;
std::map<const char*, Texture2D>    Assets::textures;

Shader Assets::get_shader(const char* name)
{
	return shaders[name];
}

Texture2D Assets::get_texture(const char* name)
{
	return textures[name];
}

void Assets::dispose()
{
	for (auto s : shaders)
		glDeleteProgram(s.second.id);

	for (auto t : textures)
		glDeleteTextures(1, &t.second.id);
}