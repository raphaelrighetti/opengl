#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <gl/glew.h>

class Shader
{
public:
	Shader();
	Shader(const char* vShaderPath, const char* fShaderPath);
	~Shader();
	void CreateProgram(const char* vShaderPath, const char* fShaderPath);
	void UseProgram();
	void ClearProgram();
	GLuint GetUniModel();
	GLuint GetUniProjection();

private:
	GLuint programID, uniModel, uniProjection;
};
