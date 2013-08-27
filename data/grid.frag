#version 150 core

in vec3 position;

out vec4 fragColor;

float linearize(float depth);

void main()
{
	float z = linearize(gl_FragCoord.z);	
    // fragColor = vec4(0.0,0.0,0,gl_FragCoord.z);   
    if (gl_PrimitiveID == 1)
    	fragColor = vec4(0,0,0,z);
    else
    	fragColor = vec4(0.8,0.8,0.8,z);	

}
