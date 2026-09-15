#version 330

layout (location = 0) in vec3 pos;

uniform float xmov;

void main()
{
	gl_Position = vec4(0.4*pos.x+xmov, 0.4*pos.y, 0.4*pos.z, 1.0);
}