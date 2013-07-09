#version 150 core

uniform mat4 transform;
uniform mat4 LightSourceTransform;
uniform mat4 invCameraTransform;

in vec3 a_vertex;
in vec3 a_normal;

out vec3 normal;
out vec4 shadowCoord;


void main(void) 
{
    normal = a_normal;
	gl_Position = transform * vec4(a_vertex, 1.0);
	shadowCoord = LightSourceTransform * invCameraTransform * transform * vec4(a_vertex, 1.0);
}
