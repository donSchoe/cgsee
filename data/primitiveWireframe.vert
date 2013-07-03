#version 150 core

in vec3 a_vertex;
in vec3 a_normal;
in vec2 a_texc;

out vec2 b_texc;

uniform mat4 transform;

void main(void) {
    b_texc = a_texc;
    vec3 n = normalize(a_normal);
    gl_Position = transform * vec4(a_vertex, 1.0);
}
