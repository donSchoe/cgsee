#version 150 core

in vec3 a_vertex;

void main(void) 
{
   gl_Position = vec4(a_vertex,1);
}