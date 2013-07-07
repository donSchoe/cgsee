#version 150 core

//partly from http://www.opengl.org/sdk/docs/tutorials/TyphoonLabs/Chapter_4.pdf

out vec4 fragColor;

in vec2 texc;
in vec3 normal;
in vec3 position;

uniform vec3 cameraposition;
uniform vec3 lightdir;
uniform vec3 lightdir2;
uniform mat4 light;
uniform mat4 light2;
uniform vec4 lightambientglobal;

uniform sampler2D diffuseTex;

vec4 phongLighting(vec3 n, vec3 v_pos, vec3 cameraposition, vec3 lightdir, vec3 lightdir2, mat4 light, mat4 light2, vec4 lightambientglobal, vec2 texc);

void main()
{
    fragColor = phongLighting(normal, position, cameraposition, lightdir, lightdir2, light, light2, lightambientglobal, texc);
}
