#version 330

in vec4 gl_FragCoord;
out vec4 fragColor;

in vec3 normal;

float linearize(float depth);

void main()
{
	vec3 n = normalize(normal);
	n *= 0.5;
	n += 0.5;

	float z = linearize(gl_FragCoord.z);

	fragColor = vec4(n, z);
}
