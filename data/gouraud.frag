#version 150 core

in vec4 gl_FragCoord;
out vec4 gl_FragColor;
in vec4 color;

void main()
{
	gl_FragColor=color;
}