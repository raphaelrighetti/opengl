#version 330

out vec4 color;

in vec4 vColor;

uniform vec3 cppColor;

void main()
{
	color = vColor;
}