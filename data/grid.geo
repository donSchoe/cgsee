#version 150 core

layout (triangles) in;
layout (line_strip, max_vertices = 200) out;

uniform mat4 transform;

const vec3 hbase[2] =vec3[2](vec3(50,0,0), vec3(-50,0,0));
const vec3 vbase[2] =vec3[2](vec3(0,0,50), vec3(0,0,-50));

void main(void) {

    int number = 0;
    for(int k; k < 50; k++){
    	for(int i = 0; i < hbase.length(); i++){
    		gl_Position = transform * (vec4(hbase[i],1) + k * vec4(0,0,1,0) - vec4(0,0,25,0));
    	//	gl_Position.z *= 50;
    		EmitVertex();

    	}
    	EndPrimitive();
	} 

	   for(int k; k < 50; k++){
    	for(int i = 0; i < vbase.length(); i++){
    		gl_Position = transform * (vec4(vbase[i],1) + k * vec4(1,0,0,0) -vec4(25,0,0,0));
    	//	gl_Position.z *= 50;
    		EmitVertex();

    	}
    	EndPrimitive();
	}       
}