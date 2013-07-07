#version 150

layout(triangles) in;
layout(line_strip, max_vertices = 2) out;

in vec3 o_normal[3];
in vec4 pos[3];

out float relToPeak;

void main(void) {
    for(int i = 0; i < gl_in.length(); i++) {
        vec4 baseVertex, peakVertex;

        baseVertex = pos[i];
        peakVertex = baseVertex + vec4(normalize(o_normal[i]), 0.0)/5;

        gl_Position = baseVertex;
        relToPeak = 0.0;
        EmitVertex();

        gl_Position = peakVertex;
        relToPeak = 1.0;
        EmitVertex();

        EndPrimitive();
    }
}

