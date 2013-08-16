#version 150 core

out vec4 fragcolor;
in vec2 v_uv;

uniform sampler2D source;
uniform sampler2D shadows;
uniform sampler2D ssao;
uniform sampler2D grid;


void main(){
	if (texture(grid, v_uv).a < texture(source, v_uv).a)
		fragcolor =  texture(source, v_uv) * min(texture(shadows, v_uv)+0.2, texture(ssao, v_uv));
	else 
		fragcolor = texture(grid, v_uv);
}