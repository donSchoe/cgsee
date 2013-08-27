#version 150 core

in vec3 a_vertex;
in vec3 a_normal;
in vec3 a_texc;

uniform mat4 transform;
uniform mat4 view;

uniform float znear;
uniform float zfar;

out vec3 normal;
out vec3 position;
out vec3 texc;

void main(void)
{
	normal = normalize(a_normal);
	texc = a_texc;
	gl_Position = transform * vec4(a_vertex, 1.0);
	//position = vec3(view * vec4(a_vertex, 1.0));
}
//#version 150 core
//
//in vec3 a_vertex;
//in vec3 a_normal;
//in vec3 a_texc;
//
//uniform mat4 transform;
//uniform mat4 view;
//
//out vec3 normal;
//out vec3 position;
//out vec3 texc;
//
//uniform sampler2D texture;
//
//void main(void)
//{
//	normal = normalize(a_normal);
//	texc = a_texc;
//	gl_Position = transform * vec4(a_vertex, 1.0);
//	position = vec3(view * vec4(a_vertex, 1.0));
//}
