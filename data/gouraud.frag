#version 150 core

in vec4 gl_FragCoord;
out vec4 fragColor;
in vec4 color;

void main()
{
	fragColor=color;
}
