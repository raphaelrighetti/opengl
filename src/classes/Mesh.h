#pragma once

#include <GL/glew.h>

class Mesh
{
public:
	Mesh();
	~Mesh();

	void CreateMesh(GLfloat* vertices, GLuint* elements, 
		unsigned int vertexNum, unsigned int elementNum);
	void RenderMesh();
	void ClearMesh();

private:
	GLuint VAO, VBO, EBO;
	GLsizei elementCount;
};
