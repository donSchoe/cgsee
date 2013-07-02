#version 150 core

in vec3 position;

out vec4 fragColor;

void main()
{
	float x = fract(position.x);
	float z = fract(position.z);

    float x1 = smoothstep (x, 0.99);
    float z1 = smoothstep (z, 0.99);

    fragColor = (z1 * x1) * vec4(1,1,1,1);
}
