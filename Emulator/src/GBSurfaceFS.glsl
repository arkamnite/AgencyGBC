#version 330 core

out FragColour;
in vec3 ourColour;

void main()
{
	FragColour = vec4(ourColour, 1.0f);
}