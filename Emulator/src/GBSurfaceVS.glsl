#version 330 core

layout(location = 0) in vec2 aPos;

out vec3 ourColour;

void main()
{
	gl_Position = vec4(aPos, 0, 0);
	ourColour = vec3(aPos, 0);
}