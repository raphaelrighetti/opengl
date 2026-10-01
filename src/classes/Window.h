#pragma once

#include <iostream>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(GLint width, GLint height);

	void Init();
	GLFWwindow* GetWindow() { return window; }
	GLint GetBufferWidth() { return bufferWidth; }
	GLint GetBufferHeight() { return bufferHeight; }
	bool GetShouldClose() { return glfwWindowShouldClose(window); }
	void SwapBuffers() { glfwSwapBuffers(window); }
	void ClearWindow();

	~Window();

private:
	GLFWwindow* window;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
