#include "Shader.h"
#include "../utils/fs.h"

Shader::Shader()
{
	programID = 0;
	uniModel = 0;
	uniProjection = 0;
}

Shader::Shader(const char* vShaderPath, const char* fShaderPath)
{
	programID = 0;
	uniModel = 0;
	uniProjection = 0;
}

void Shader::CreateProgram(const char* vShaderPath, const char* fShaderPath)
{
	std::string vsc{ fs::ReadFile(vShaderPath) };
	std::string fsc{ fs::ReadFile(fShaderPath) };
	const GLchar* vShaderCode[]{ vsc.c_str() };
	const GLchar* fShaderCode[]{ fsc.c_str() };

	programID = glCreateProgram();

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
		glDeleteProgram(programID);
		return;
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
		glDeleteProgram(programID);
		return;
	}

	glAttachShader(programID, vShader);
	glAttachShader(programID, fShader);

	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, 1024, nullptr, log);
		std::cout << "Falhou em linkar o program..." << std::endl;
		std::cout << log << std::endl;
		glDeleteShader(vShader);
		glDeleteShader(fShader);
		glDeleteProgram(programID);
		return;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	uniModel = glGetUniformLocation(programID, "model");
	uniProjection = glGetUniformLocation(programID, "projection");
}

void Shader::UseProgram()
{
	glUseProgram(programID);
}

void Shader::ClearProgram()
{
	if (programID != 0)
	{
		glDeleteProgram(programID);
		programID = 0;
	}

	programID = 0;
	uniModel = 0;
	uniProjection = 0;
}

GLuint Shader::GetUniModel()
{
	return uniModel;
}

GLuint Shader::GetUniProjection()
{
	return uniProjection;
}

Shader::~Shader()
{
	ClearProgram();
}
