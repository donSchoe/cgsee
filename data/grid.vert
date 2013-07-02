#version 150 core

in vec3 a_vertex;


uniform mat4 transform;
uniform float zNear;
uniform float zFar;

out vec3 position;

void main(void) {
    vec3 n = vec3(0,1,0);
    float size= 10;
    gl_Position = transform * vec4(a_vertex * size, 1.0);
    position = a_vertex.xyz * size;
}
