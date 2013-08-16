#version 150 core

uniform ivec2 viewport;
uniform vec2 samples[128];
uniform int sample_count;

uniform float lightSize;
uniform float searchWidth;
uniform float zOffset;

uniform sampler2D shadowMap;

//in vec4 fragCoord;
in vec4 shadowCoord;

out vec4 fragColor;

float linearize(float depth);

float average_blocker_depth(vec2 coord, float zReceiver, float viewportsize) {
    float zSum = 0.0;
    int numBlockers = 0;
    for(int i=0; i<sample_count; i++){
        float zBlocker = texture(shadowMap, coord + samples[i] * searchWidth / viewportsize).z;
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
    
    float z = linearize(coord.z) - zOffset;

    float viewportsize = (viewport.x+viewport.y)/2.0;

    float zBlocker = average_blocker_depth(coord.xy, z, viewportsize);

    if(zBlocker == 0.0) {
        fragColor = vec4(1);
        return;
    }


    float penumbra = (z - zBlocker) / zBlocker * lightSize / viewportsize;
    float shadow = 0.0;
    float x,y;
    for (int i=0; i<sample_count; i++) { 
            vec2 offset = coord.st + samples[i] * penumbra;
            float distanceFromLight = texture(shadowMap, offset).z;
            shadow += distanceFromLight <= z ? 0.0 : 1.0;
    }

    fragColor = vec4(vec3(shadow / sample_count), 1.0);
    
}
