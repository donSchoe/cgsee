#version 150 core

uniform ivec2 viewport;
uniform float znear;
uniform float zfar;
uniform vec2 samples[128];
uniform int sample_count;

const float lightSize =   0.015; // make uniform
const float searchWidth = 0.01; // make uniform
const float zOffset = 0.002; // make uniform

uniform sampler2D shadowMap;

//in vec4 fragCoord;
in vec4 normal;
in vec4 shadowCoord;

out vec4 fragColor;

float average_blocker_depth(vec2 coord, float zReceiver) {
    float zSum = 0.0;
    int numBlockers = 0;
    for(int i=0; i<sample_count; i++){
        float zBlocker = texture(shadowMap, coord + samples[i]*searchWidth).z;
        if (zBlocker < zReceiver){
            zSum += zBlocker;
            numBlockers++;
        } 
    }
    return numBlockers > 0 ? zSum/numBlockers : 0.0;
}

void main()
{
    vec2 uv = gl_FragCoord.xy / viewport;

	vec4 coord = shadowCoord / shadowCoord.w;
    
    float z = coord.z - zOffset;
    z = - znear * z / (zfar * z - zfar - znear * z);

    float zBlocker = average_blocker_depth(coord.xy, z);

    if(zBlocker == 0.0) {
        fragColor = vec4(1);
        return;
    }

    float penumbra = (z - zBlocker) / zBlocker * lightSize;
    float shadow = 0.0;
    float x,y;
    for (int i=0; i<sample_count; i++) { 
            vec2 offset = coord.st + samples[i] * penumbra;
            float distanceFromLight = texture(shadowMap, offset).z;
            shadow += distanceFromLight <= z ? 0.0 : 1.0;
    }

    fragColor = vec4(vec3(shadow / sample_count), 1.0);
    
}
