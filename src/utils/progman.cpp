#include "progman.h"
#include "fs.h"

#include <string>
#include <iostream>

GLuint progman::CreateProgram(const char* vShaderPath, const char* fShaderPath)
{
	std::string vsc{ fs::ReadFile(vShaderPath) };
	std::string fsc{ fs::ReadFile(fShaderPath) };
	const GLchar* vShaderCode[]{ vsc.c_str() };
	const GLchar* fShaderCode[]{ fsc.c_str() };

	GLuint program{ glCreateProgram() };

	GLuint vShader{ glCreateShader(GL_VERTEX_SHADER) };
	glShaderSource(vShader, 1, vShaderCode, nullptr);
	GLuint fShader{ glCreateShader(GL_FRAGMENT_SHADER) };
	glShaderSource(fShader, 1, fShaderCode, nullptr);

	GLint success{};
	GLchar log[1024]{};

	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vShader, 1024, nullptr, log);
		std::cout << "Deu merda na vShader..." << std::endl;
		std::cout << log << std::endl;
		glDeleteShader(vShader);
		glDeleteShader(fShader);
		glDeleteProgram(program);
		return 0;
	}
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fShader, 1024, nullptr, log);
		std::cout << "Deu merda na fShader..." << std::endl;
		std::cout << log << std::endl;
		glDeleteShader(vShader);
		glDeleteShader(fShader);
		glDeleteProgram(program);
		return 0;
	}

	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 1024, nullptr, log);
		std::cout << "Falhou em linkar o program..." << std::endl;
		std::cout << log << std::endl;
		glDeleteShader(vShader);
		glDeleteShader(fShader);
		glDeleteProgram(program);
		return 0;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	return program;
}