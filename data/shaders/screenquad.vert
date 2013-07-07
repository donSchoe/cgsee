#version 150 core

in vec3 a_vertex;

uniform mat4 projectionInverse;

out vec2 v_uv;

void main(void)
{
	v_uv = a_vertex.xy * 0.5 + 0.5;
	gl_Position = vec4(a_vertex, 1.0);
}
