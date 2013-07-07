#version 150 core
//
// Vertex shader for Gooch shading
//
// Author: Randi Rost
//
// Copyright (c) 2002-2005 3Dlabs Inc. Ltd.
//

uniform mat4 transform;
uniform mat4 view;
uniform sampler2D texture;

in vec3 a_vertex;
in vec3 a_normal;
in vec2 a_texc;
out vec3 normal;
out vec3 position;
out vec2 texc;

void main()
{
    normal = a_normal;
    texc = a_texc;
    gl_Position = transform * vec4(a_vertex, 1.0);
    position = vec3(view * vec4(a_vertex, 1.0));
}
