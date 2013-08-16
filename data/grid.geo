#version 150 core

layout (triangles) in;
layout (line_strip, max_vertices = 200) out;

uniform mat4 transform;



const vec4 horizontalDirection 	= vec4(0, 0, 1, 0);
const vec4 verticalDirection 	= vec4(1, 0, 0, 0);

const int lines = 19;

void main(void) {
	float halflines = floor(lines/2);
	vec3 horizontalBase[2] =	vec3[2](vec3(halflines,0,0), vec3(-halflines,0,0));
	vec3 verticalBase[2]   =	vec3[2](vec3(0,0,halflines), vec3(0,0,-halflines));

	for(int k; k < lines; k++){

	 	//draw horizontal lines
    	gl_Position = transform * (vec4(horizontalBase[0],1) + k * horizontalDirection - halflines * horizontalDirection);
    	EmitVertex();
    	gl_Position = transform * (vec4(horizontalBase[1],1) + k * horizontalDirection - halflines* horizontalDirection);
    	EmitVertex();
    	EndPrimitive(); 

    	//draw vertical lines
    	gl_Position = transform * (vec4(verticalBase[0],1) + k * verticalDirection - halflines * verticalDirection);
    	EmitVertex();
    	gl_Position = transform * (vec4(verticalBase[1],1) + k * verticalDirection - halflines * verticalDirection);
    	EmitVertex();
    	EndPrimitive(); 
    }
}