#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::string;

class Shader{
	public:
	Shader(const string& vPath, const string& fPath);
	void use();

	void setFloat(const char* name, float value);
	void setFloat(const string& name, float value);
	void setInt(const char* name, int value);
	void setInt(const string&  name, int value);
	void setMat4(const char* name, const glm::mat4& matrix);
	void setVec3(const char* name, const glm::vec3& vec, int count = 1);
	void setVec3(const string& name, const glm::vec3& vec, int count = 1);
	void setVec4(const char* name, const glm::vec4& vec, int count = 1);

	public:
	unsigned int ID;
	private:
	string parseShader(const string& path);
	
};
