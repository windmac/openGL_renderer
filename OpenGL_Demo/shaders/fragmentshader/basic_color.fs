#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec4 ourColor; // we set this variable in the OpenGL code.
void main(){

	FragColor = ourColor;
	} 