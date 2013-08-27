#version 150 core

in vec4 gl_FragCoord;
out vec4 fragColor;

float linearize(float depth);

void main()
{
    float z = linearize(gl_FragCoord.z);

	fragColor = vec4(z);
}

