#version 150 core

out vec4 fragcolor;
in vec2 v_uv;

uniform sampler2D source;
uniform sampler2D shadows;
uniform sampler2D ssao;


void main()
{
    fragcolor = texture(source, v_uv)* min(texture(shadows, v_uv)+0.2, texture(ssao, v_uv));
}
