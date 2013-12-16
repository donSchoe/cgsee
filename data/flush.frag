#version 150 core

out vec4 fragcolor;
in vec2 v_uv;

uniform sampler2D source;
uniform sampler2D shadows;
uniform sampler2D ssao;
uniform sampler2D grid;
uniform sampler2D depth;

void main(){
	// shorter version, according to glreference card this should work, but mix(vec4,vec4,bool) is yet unsupported
	// vec4 gridcolor = mix(vec4(1), texture(grid,v_uv), lessThan(texture(depth,v_uv).a, texture(grid,v_uv).a));
	// fragcolor =  texture(source, v_uv) * min(texture(shadows, v_uv)+0.2, texture(ssao, v_uv)) * gridcolor;

	if (texture(depth,v_uv).a - 0.01 > texture(grid,v_uv).a)
		fragcolor =  texture(source, v_uv) * min(texture(shadows, v_uv)+0.2, texture(ssao, v_uv)) * texture(grid, v_uv);
	else
		fragcolor =  texture(source, v_uv) * min(texture(shadows, v_uv)+0.2, texture(ssao, v_uv));
 }