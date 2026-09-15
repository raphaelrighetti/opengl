#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Windows.h>

//int uniXmov{};

std::string ReadFile(const char* path)
{
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

GLuint CreateProgram()
{
	std::string vsc{ ReadFile("../shaders/basic.vert").c_str() };
	std::string fsc{ ReadFile("../shaders/basic.frag").c_str() };
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
		glGetShaderInfoLog(vShader, 1024, NULL, log);
		std::cout << "Deu merda na vShader..." << std::endl;
		std::cout << log << std::endl;
		return 0;
	}
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fShader, 1024, NULL, log);
		std::cout << "Deu merda na fShader..." << std::endl;
		std::cout << log << std::endl;
		return 0;
	}

	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 1024, NULL, log);
		std::cout << "Falhou em linkar o program..." << std::endl;
		std::cout << log << std::endl;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	return program;
}

GLuint CreateTriangle()
{
	GLfloat vertices[]{
		-1.0, -1.0, 0.0,
		1.0, -1.0, 0.0,
		0.0, 1.0, 0.0
	};

	GLuint VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	return VAO;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n";
		return -1;
	}

	// Request an OpenGL 3.3 core profile context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	const int width = 800;
	const int height = 600;
	GLFWwindow* window = glfwCreateWindow(width, height, "Raphinha games", NULL, NULL);
	if (!window) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize GLEW after creating an OpenGL context
	glewExperimental = GL_TRUE;
	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewStatus) << "\n";
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

	// Set the viewport
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	glViewport(0, 0, fbWidth, fbHeight);

	GLuint triangleVAO{ CreateTriangle()};
	GLuint program{ CreateProgram() };
	GLint uniXmov{ glGetUniformLocation(program, "xmov")};

	bool triX_Direction{ true };
	float triX_Offset{ 0.0f };
	float triX_MaxOffset{ 0.7f };
	float triX_Speed{ 0.0005f };

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		// Input: close on ESC
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
			std::cout << "ESC pressed, closing window.\n";
		}

		if (triX_Direction)
		{
			triX_Offset += triX_Speed;
		}
		else
		{
			triX_Offset -= triX_Speed;
		}

		if (abs(triX_Offset) >= triX_MaxOffset)
			triX_Direction = !triX_Direction;

		glUniform1f(uniXmov, triX_Offset);

		// Render
		glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(triangleVAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap buffers and poll events
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
