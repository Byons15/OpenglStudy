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

	void SetVec3(const std::string& name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	void SetVec3(const std::string& name, GLfloat value[])
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value);
	}

private:
	unsigned int ID;
};

