#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
}

Window::Window(GLint width, GLint height)
{
	this->width = width;
	this->height = height;
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}

void Window::Init()
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW\n";
		return;
	}

	// Request an OpenGL 3.3 core profile context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(width, height, "Cool Window ;)", NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize GLEW after creating an OpenGL context
	glewExperimental = GL_TRUE;
	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewStatus) << "\n";
		glfwDestroyWindow(window);
		glfwTerminate();
		return;
	}

	glEnable(GL_DEPTH_TEST);

	std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
	glViewport(0, 0, bufferWidth, bufferHeight);
}

void Window::ClearWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
	window = 0;
	width = 0;
	height = 0;
	bufferWidth = 0;
	bufferHeight = 0;
}

Window::~Window()
{
	ClearWindow();
}
