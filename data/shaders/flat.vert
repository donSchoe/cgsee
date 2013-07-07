#version 150 core

in vec3 a_vertex;
in vec3 a_normal;
in vec2 a_texc;

uniform mat4 transform;
uniform mat4 view;

out vec3 _normal;
out vec3 _position;
out vec2 b_texc;

void main(void)
{
    _normal = normalize(a_normal);
    b_texc = a_texc;
    gl_Position = transform * vec4(a_vertex, 1.0);
    _position = vec3(view * vec4(a_vertex, 1.0));
}
