#include "fs.h"

#include <iostream>
#include <fstream>
#include <filesystem>

std::string fs::ReadFile(const char* path)
{
	std::string cwd{ };
	std::cout << "CWD: " << std::filesystem::current_path() << std::endl;
	std::cout << "Caminho do arquivo: " << path << std::endl;

	std::ifstream file{ path };

	if (!file.is_open())
	{
		std::cerr << "Arquivo não abriu..." << std::endl;
		return "";
	}

	std::string content{ std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>() };
	std::cout << content << std::endl;
	std::cout << content << std::endl;

	file.close();
	return content;
}