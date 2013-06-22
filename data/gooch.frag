#version 150 core
//
// Fragment shader for Gooch shading
//
// Author: Randi Rost
//
// Copyright (c) 2002-2005 3Dlabs Inc. Ltd.
//

uniform mat4 transform;
uniform mat4 view;
uniform vec3 lightposition;

uniform mat4 warmcoldcolor;

in vec3 normal;
in vec3 position;


out vec4 fragColor;

void main()
{
    vec3 tnorm = normal;

    vec3 lightVec = normalize(lightposition - position);
    vec3 ReflectVec = normalize(reflect(-lightVec, tnorm));
    vec3 ViewVec = normalize(-position);
    float NdotL = (dot(lightVec, tnorm) + 1.0) * 0.5;
    
    vec3 SurfaceColor=vec3(warmcoldcolor[0]);
    vec3 WarmColor=vec3(warmcoldcolor[1]);
    vec3 CoolColor=vec3(warmcoldcolor[2]);
    float DiffuseWarm=warmcoldcolor[3].x;
    float DiffuseCool=warmcoldcolor[3].y;
    
    vec3 kcool = min(CoolColor + DiffuseCool * SurfaceColor, 1.0);
    vec3 kwarm = min(WarmColor + DiffuseWarm * SurfaceColor, 1.0);
    vec3 kfinal = mix(kcool, kwarm, NdotL);

    vec3 nreflect = normalize(ReflectVec);
    vec3 nview = normalize(ViewVec);

    float spec = max(dot(nreflect, nview), 0.0);
    spec = pow(spec, 50.0);

    fragColor = vec4(min(kfinal + spec, 1.0), 1.0);
}