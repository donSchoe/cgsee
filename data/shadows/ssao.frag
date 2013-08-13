#version 150 core

// source http://john-chapman-graphics.blogspot.de/2013/01/ssao-tutorial.html


uniform sampler2D normalz;

uniform ivec2 viewport;
uniform vec3 kernel[128];
uniform vec3 noise[16];
uniform int sample_count;
uniform float zOffset;
uniform float filterRadius;

in vec2 v_uv;
out vec4 fragcolor;


int calcNoiseCoord(vec2 v, int width, int height) {
    int x = int(v.x * viewport.x) % width;
    int y = int(v.y * viewport.y) % height;
    return x + y * width;
}

void main()
{
	vec4 normalz_value = texture(normalz, v_uv);
    
    if(normalz_value.a == 1.0) {
        fragcolor = vec4(1.0);
        return;
    }

    float radius = filterRadius / ((viewport.x+viewport.y)/2.0);

    vec3 origin = vec3(v_uv, normalz_value.a);
    vec3 normal = normalize(normalz_value.xyz * 2.0 - 1.0);
 
    // create matrix to rotate 
    vec3 rvec = noise[calcNoiseCoord(v_uv, 4, 4)];
    vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for (int i = 0; i < sample_count; ++i) {
        // get sample position:
        vec3 sample = tbn * kernel[i];
        sample = sample * radius + origin;

        // project sample position:  --- I dont know what the purpose of this is
        //vec4 offset = vec4(sample, 1.0);
        //offset = uProjectionMat * offset;
        //offset.xy /= offset.w;
        //offset.xy = offset.xy * 0.5 + 0.5;
      
        // get sample depth:
        float sampleDepth = texture(normalz, sample.xy).a;
      
        // range check & accumulate:
        float rangeCheck = abs(origin.z - sampleDepth) < radius ? 1.0 : 0.0;
        occlusion += (sampleDepth <= sample.z - zOffset ? 1.0 : 0.0) * rangeCheck;
    }
    fragcolor = vec4(1.0 - (occlusion / sample_count));
}