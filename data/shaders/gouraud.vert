#version 150 core


//partly from http://www.opengl.org/sdk/docs/tutorials/TyphoonLabs/Chapter_4.pdf

in vec3 a_vertex;
in vec3 a_normal;
in vec2 a_texc;
out vec4 color;
out vec2 texc;

uniform mat4 transform;
uniform mat4 view;

uniform vec3 cameraposition;
uniform vec3 lightdir;
uniform vec3 lightdir2;

uniform mat4 light;
uniform mat4 light2;
uniform mat4 material;
uniform vec4 lightambientglobal;

vec4 phongLighting(vec3 n, vec3 v_pos, vec3 cameraposition, vec3 lightdir, vec3 lightdir2, mat4 light, mat4 light2, vec4 lightambientglobal, vec2 texc);

void main()
{
    vec3 n = normalize(a_normal);
	gl_Position = transform * vec4(a_vertex, 1.0);
	vec3 position = vec3(view * vec4(a_vertex, 1.0));
    texc = a_texc;
	color = phongLighting(n, position, cameraposition, lightdir, lightdir2, light, light2, lightambientglobal, texc);
}
