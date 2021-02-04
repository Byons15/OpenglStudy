#pragma once
#include <glad\glad.h>

#include <string>

class Shader
{
public:
	virtual ~Shader();

	
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath);

	void Use()
	{
		glUseProgram(ID);
	}

	void SetBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void SetInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void SetFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void SetMat4(const std::string &name, float value[])
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
	}

private:
	unsigned int ID;
};

