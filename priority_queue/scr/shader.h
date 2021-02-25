#pragma once
#include<glm/glm.hpp>
#include<string>
class Shader
{
public:
	unsigned int id;
	Shader(const char* vertexPath,const char* fragmentPath);
	Shader() {	};
	void use();
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setMat4(const std::string &name, const glm::mat4& mat) const;
private:
	void checkCompileErrors(unsigned int shader, const char* type);
};

