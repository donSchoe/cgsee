#version 150 core

in vec3 position;

out vec4 fragColor;

float linearize(float depth);

void main()
{
    // fragColor = vec4(0.0,0.0,0,gl_FragCoord.z);   
    fragColor = vec4(linearize(gl_FragCoord.z));	
}
