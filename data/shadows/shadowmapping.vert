//#version 150 core
//
//uniform mat4 transform;
//uniform mat4 LightSourceTransform;
//uniform mat4 invCameraTransform;
//
//in vec3 a_vertex;
//in vec3 a_normal;
//
//out vec3 normal;
//out vec4 shadowCoord;
//
//
//void main(void)
//{
//    normal = a_normal;
//	gl_Position = transform * vec4(a_vertex, 1.0);
//	shadowCoord = LightSourceTransform * invCameraTransform * transform * vec4(a_vertex, 1.0);
//}

#version 150 core

in vec3 a_vertex;
in vec3 a_normal;
in vec3 a_texc;

uniform mat4 transform;
uniform mat4 LightSourceTransform;
uniform mat4 invCameraTransform;
uniform mat4 view;

out vec3 normal;
out vec3 position;
out vec3 texc;
out vec4 shadowCoord;

uniform sampler2D texture;

void main(void)
{
	normal = normalize(a_normal);
	texc = a_texc;
	gl_Position = transform * vec4(a_vertex, 1.0);
	position = vec3(view * vec4(a_vertex, 1.0));
	shadowCoord = LightSourceTransform * invCameraTransform * transform * vec4(a_vertex, 1.0);
}
