#version 150 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 vTexPos;

uniform mat4 transform;
uniform sampler2D texture;

out vec2 fTexPos;

void main(void) {
    fTexPos = vTexPos;
    gl_Position = transform * vec4(inPos, 1);
}
