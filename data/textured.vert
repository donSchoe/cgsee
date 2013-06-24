#version 150 core

in vec3 a_vertex;
in vec3 a_normal;

uniform mat4 transform;

void main(void) {
    vec3 n = normalize(a_normal);
    gl_Position = transform * vec4(a_vertex, 1.0);
}
