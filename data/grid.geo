#version 150 core

layout (triangles) in;
layout (line_strip, max_vertices = 600) out;

uniform mat4 transform;

const int lines = 16;
const int sublines = 5;

void main(void) {
	float halflines = floor(lines/2);
    float startingpoint = halflines * sublines;

    vec4 horizontalDirection  = vec4(0, 0, 1.0/sublines, 0);
    vec4 verticalDirection    = vec4(1.0/sublines, 0, 0, 0);

	vec3 horizontalBase[2] =	vec3[2](vec3(halflines,0,0), vec3(-halflines,0,0));
	vec3 verticalBase[2]   =	vec3[2](vec3(0,0,halflines), vec3(0,0,-halflines));

	for(int k; k <= lines * sublines; k++){

        if (mod(k,sublines) == 0)
            gl_PrimitiveID = 1;
        else 
            gl_PrimitiveID = 0;

	 	//draw horizontal lines
    	gl_Position = transform * (vec4(horizontalBase[0],1) + k * horizontalDirection - startingpoint *  horizontalDirection);
    	EmitVertex();
    	gl_Position = transform * (vec4(horizontalBase[1],1) + k * horizontalDirection - startingpoint * horizontalDirection);
    	EmitVertex();
    	EndPrimitive(); 

    	//draw vertical lines
    	gl_Position = transform * (vec4(verticalBase[0],1) + k * verticalDirection - startingpoint * verticalDirection);
    	EmitVertex();
    	gl_Position = transform * (vec4(verticalBase[1],1) + k * verticalDirection - startingpoint* verticalDirection);
    	EmitVertex();
    	EndPrimitive(); 
    }
}