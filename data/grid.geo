#version 150 core

layout (triangles) in;
layout (line_strip, max_vertices = 600) out;

uniform mat4 transform;

const vec4 horizontalDirection 	= vec4(0, 0, 0.2, 0);
const vec4 verticalDirection 	= vec4(0.2, 0, 0, 0);

const int lines = 16;

void main(void) {
	float halflines = floor(lines/2);
	vec3 horizontalBase[2] =	vec3[2](vec3(halflines,0,0), vec3(-halflines,0,0));
	vec3 verticalBase[2]   =	vec3[2](vec3(0,0,halflines), vec3(0,0,-halflines));

	for(int k; k < lines * 5 + 1; k++){

        if (mod(k,5) == 0)
            gl_PrimitiveID = 1;
        else 
            gl_PrimitiveID = 0;

	 	//draw horizontal lines
    	gl_Position = transform * (vec4(horizontalBase[0],1) + k * horizontalDirection - halflines * 5 * horizontalDirection);
    	EmitVertex();
    	gl_Position = transform * (vec4(horizontalBase[1],1) + k * horizontalDirection - halflines * 5 * horizontalDirection);
    	EmitVertex();
    	EndPrimitive(); 

    	//draw vertical lines
    	gl_Position = transform * (vec4(verticalBase[0],1) + k * verticalDirection - halflines * 5 * verticalDirection);
    	EmitVertex();
    	gl_Position = transform * (vec4(verticalBase[1],1) + k * verticalDirection - halflines * 5 * verticalDirection);
    	EmitVertex();
    	EndPrimitive(); 
    }
}