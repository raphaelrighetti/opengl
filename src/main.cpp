#include "utils/fs.h"
//#include "utils/progman.h"
#include "classes/Mesh.h"
#include "classes/Shader.h"
#include "classes/Window.h"

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Windows.h>

std::vector<Mesh*> meshes{};

void CreateTriangle()
{
	GLuint elements[]{
		0, 2, 3,
		1, 2, 3,
		0, 2, 4,
		1, 2, 4,
		0, 1, 3,
		0, 1, 4
	};

	GLfloat vertices[]{
		// X esquerda 0
		-1.0f, -1.0f, 0.0f,
		// X direita 1
		1.0f, -1.0f, 0.0f,
		// Y cima 2
		0.0f, 1.0f, 0.0f,
		// Z frente 3
		0.0f, -1.0f, 1.0f,
		// Z trás 4
		0.0f, -1.0f, -1.0f
	};

	Mesh* mesh{ new Mesh() };
	mesh->CreateMesh(vertices, elements, 18, 18);

	Mesh* mesh2{ new Mesh() };
	mesh->CreateMesh(vertices, elements, 18, 18);

	meshes.push_back(mesh);
	meshes.push_back(mesh2);
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);

	Window window{ 1280, 720 };
	window.Init();

	std::vector<Shader> programs{};
	Shader shader;
	shader.CreateProgram("shaders/basic.vert", "shaders/basic.frag");
	programs.push_back(shader);

	float xLimit{ 0.7f };
	float xSpeed{ 0.0005f };
	float xCurrValue{ 0.0f };

	float rotationAngle{ 0.0f };
	float rotationAmount{ 0.05f };

	float scaleMax{ 0.8f }, scaleMin{ 0.1 };
	float scaleCurrent{ scaleMin };
	float scaleAmount{ 0.0001f };

	float colorMin{ 0.0f }, colorMax{ 1.0f };
	float colorCurrent{ 0.0f };
	float colorAmount{ 0.0001f };

	glm::mat4 projection{ glm::perspective(90.0f, (GLfloat)window.GetBufferWidth() / (GLfloat)window.GetBufferHeight(),
		0.1f, 100.0f) };
	
	CreateTriangle();

	// Main loop
	while (!window.GetShouldClose()) 
	{
		// Input: close on ESC
		if (glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window.GetWindow(), true);
			std::cout << "ESC pressed, closing window.\n";
		}

		// Render
		glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		programs[0].UseProgram();

		glm::mat4 model{ 1.0f };
		model = glm::translate(model, glm::vec3(xCurrValue, 1.0f, -2.5f));
		model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		/*model = glm::scale(model, glm::vec3(scaleCurrent, scaleCurrent, scaleCurrent));*/
		glUniformMatrix4fv(programs[0].GetUniModel(), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(programs[0].GetUniProjection(), 1, GL_FALSE, glm::value_ptr(projection));

		meshes[0]->RenderMesh();

		model = glm::mat4{ 1.0 };
		model = glm::translate(model, glm::vec3(-xCurrValue, -1.0f, -2.5f));
		model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(programs[0].GetUniModel(), 1, GL_FALSE, glm::value_ptr(model));

		meshes[0]->RenderMesh();

		/*for (Mesh* mesh : meshes)
		{
			mesh->RenderMesh();
		}*/



		glUseProgram(0);

		// Swap buffers and poll events
		window.SwapBuffers();

		glfwPollEvents();
	}
	
	window.ClearWindow();
	return 0;
}
