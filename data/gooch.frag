#version 330
//
// Fragment shader for Gooch shading
//
// Author: Randi Rost
//
// Copyright (c) 2002-2005 3Dlabs Inc. Ltd.
//
// See 3Dlabs-License.txt for license information
//
//updated code to newer glsl version

/*
uniform vec3 SurfaceColor; // (0.75, 0.75, 0.75)
uniform vec3 WarmColor; // (0.6, 0.6, 0.0)
uniform vec3 CoolColor; // (0.0, 0.0, 0.6)
uniform float DiffuseWarm; // 0.45
uniform float DiffuseCool; // 0.45 
*/

in float NdotL;
in vec3 ReflectVec;
in vec3 ViewVec;

out vec4 gl_FragColor;

void main()
{
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
	spec = pow(spec, 32.0);

	gl_FragColor = vec4(min(kfinal + spec, 1.0), 1.0);
}