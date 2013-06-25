#version 150 core

in vec3 a_vertex;
in vec3 a_normal;
in vec2 a_texc;

out vec2 f_texc;

uniform mat4 transform;

uniform sampler2D texture;

void main(void) {
    f_texc = a_texc;
    gl_Position = transform * vec4(a_vertex, 1.0);
}
