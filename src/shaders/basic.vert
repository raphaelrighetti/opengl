#version 330

layout (location = 0) in vec3 pos;

out vec4 vColor;

uniform mat4 model;

void main()
{
	gl_Position = model * vec4(pos.x, pos.y, pos.z, 1.0);
	vColor = vec4(clamp(pos, 0.0, 1.0), 1.0);
}