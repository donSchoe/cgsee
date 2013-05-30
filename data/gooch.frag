#version 330
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

in vec3 normal;
in vec3 position;


out vec4 gl_FragColor;

void main()
{
	vec3 tnorm = normal;

	vec3 lightVec = normalize(lightposition - position);
	vec3 ReflectVec = normalize(reflect(-lightVec, tnorm));
	vec3 ViewVec = normalize(-position);
	float NdotL = (dot(lightVec, tnorm) + 1.0) * 0.5;
	
	vec3 SurfaceColor=vec3(0.75, 0.75, 0.75);
	vec3 WarmColor=vec3(0.6, 0.6, 0.0);
	vec3 CoolColor=vec3(0.0, 0.0, 0.6);
	float DiffuseWarm=0.45;
	float DiffuseCool=0.45;
	
	vec3 kcool = min(CoolColor + DiffuseCool * SurfaceColor, 1.0);
	vec3 kwarm = min(WarmColor + DiffuseWarm * SurfaceColor, 1.0);
	vec3 kfinal = mix(kcool, kwarm, NdotL);

	vec3 nreflect = normalize(ReflectVec);
	vec3 nview = normalize(ViewVec);

	float spec = max(dot(nreflect, nview), 0.0);
	spec = pow(spec, 50.0);

	gl_FragColor = vec4(min(kfinal + spec, 1.0), 1.0);
}