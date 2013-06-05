#version 330

in vec4 gl_FragCoord;
out vec4 gl_FragColor;

in vec3 normal;

uniform float znear;
uniform float zfar;

void main()
{
	vec3 n = normalize(normal);
	n *= 0.5;
	n += 0.5;

	float z = gl_FragCoord.z; 

	// d = (2.0 * zfar * znear / (zfar + znear - (zfar - znear) * (2.0 * z- 1.0)));
	// normalized to [0,1]
	// d = (d - znear) / (zfar - znear);

	// simplyfied with wolfram alpha
	z = - znear * z / (zfar * z - zfar - znear * z);

	gl_FragColor = vec4(n, z);
}
