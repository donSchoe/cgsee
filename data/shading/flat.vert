#version 150 core

in vec3 a_vertex;
in vec3 a_normal;

uniform mat4 transform;
uniform vec3 cameraPos;

out vec3 _normal;
out vec3 _position;

void main(void) 
{
    _normal = normalize(a_normal);
    gl_Position = transform * vec4(a_vertex, 1.0);
    //_position = vec3(view * vec4(a_vertex, 1.0));
    _position = cameraPos * a_vertex;
}