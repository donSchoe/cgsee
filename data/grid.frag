#version 150 core

in vec3 position;

out vec4 fragColor;

void main()
{
	float x = fract(position.x);
	float z = fract(position.z);

    float x1 = step (0.99,x);
    float z1 = step (0.99,z);

    fragColor = sign(1-x1-z1)*vec4(1,1,1,1);

}
