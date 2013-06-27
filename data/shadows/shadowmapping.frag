#version 150

uniform ivec2 viewport;
uniform float znear;
uniform float zfar;

//uniform sampler2D source;
uniform sampler2D shadowMap;

in vec4 gl_FragCoord;
in vec4 shadowCoord;

out vec4 fragColor;



void main()
{
    float uv = gl_FragCoord.xy / viewport;
    
    //vec4 sourceFragment = texture(source, uv);

	vec4 coord = shadowCoord / shadowCoord.w;
    
    // Used to lower moir� pattern and self-shadowing
    float z = coord.z - 0.0005;
    z = - znear * z / (zfar * z - zfar - znear * z);
    float z_blocker = texture(shadowMap, coord.st).z;

    float penumbra = (z- z_blocker) / z_blocker;
    float shadow = 0.0;
    float x,y;
    for (y = -2.5 ; y <=2.5 ; y+=1.0) // 6 rounds
        for (x = -2.5 ; x <=2.5 ; x+=1.0) { // 6 rounds
            vec2 offset = coord.st + vec2(x, y) * (1+penumbra*15) / viewport ;
            float distanceFromLight = texture(shadowMap, offset).z;
            shadow += distanceFromLight < z ? 0.0 : 1.0;
    }

    //gl_FragColor = vec4(penumbra);
    gl_FragColor = vec4(vec3(shadow/ 25.0), 1.0);

}
