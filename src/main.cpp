#include "utils/fs.h"
#include "utils/progman.h"

#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Windows.h>

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

	GLuint triangleVAO{ CreateTriangle() };
	GLuint program
	{
		progman::CreateProgram("shaders/basic.vert", "shaders/basic.frag") 
	};

	float xLimit{ 0.7f };
	float xSpeed{ 0.0005f };
	float xCurrValue{ 0.0f };

	float rotationAngle{ 0.0f };
	float rotationAmount{ 0.01f };

	float scaleMax{ 0.8f }, scaleMin{ 0.1 };
	float scaleCurrent{ scaleMin };
	float scaleAmount{ 0.0001f };

	float colorMin{ 0.0f }, colorMax{ 1.0f };
	float colorCurrent{ 0.0f };
	float colorAmount{ 0.0001f };
	
	GLint uniModel{ glGetUniformLocation(program, "model") };
	GLint uniCppColor{ glGetUniformLocation(program, "cppColor") };

	// Main loop
	while (!glfwWindowShouldClose(window)) 
	{
		// Input: close on ESC
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
			std::cout << "ESC pressed, closing window.\n";
		}

		// Render
		glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(triangleVAO);

		if (xCurrValue >= xLimit)
		{
			xSpeed = -std::abs(xSpeed);
		}
		else if (xCurrValue <= -xLimit)
		{
			xSpeed = std::abs(xSpeed);
		}

		if (rotationAngle >= 360)
		{
			rotationAngle -= 360;
		}

		if (scaleCurrent >= scaleMax)
		{
			scaleAmount = -std::abs(scaleAmount);
		}
		else if (scaleCurrent <= scaleMin)
		{
			scaleAmount = std::abs(scaleAmount);
		}

		if (colorCurrent >= colorMax)
		{
			colorAmount = -std::abs(colorAmount);
		}
		else if (colorCurrent <= colorMin)
		{
			colorAmount = std::abs(colorAmount);
		}

		xCurrValue += xSpeed;
		rotationAngle += rotationAmount;
		scaleCurrent += scaleAmount;
		colorCurrent += colorAmount;

		glm::mat4 model{ 1.0f };
		model = glm::translate(model, glm::vec3(xCurrValue, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scaleCurrent, scaleCurrent, 1.0));
		glm::vec3 cppColor{ colorCurrent };

		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniCppColor, 1, glm::value_ptr(cppColor));

		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap buffers and poll events
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
