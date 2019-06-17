#version 330 core
out vec4 FragColor;
in vec4 ourColor;
uniform vec4 ourColor2; // we set this variable in the OpenGL code.
void main(){

	FragColor = ourColor/2+ourColor2/2;
	} 