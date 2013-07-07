#version 150 core

#define M_PI 3.1415926535897932384626433832795

in vec3 v_normal[];
out float edge_dist[3];

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

void _EmitVertex(vec4 position, float d1, float d2, float d3) {
    gl_Position = position;
    edge_dist[0] = d1;
    edge_dist[1] = d2;
    edge_dist[2] = d3;
    EmitVertex();
}

float dist(vec4 v1, vec4 v2, vec4 v3) {
    vec4 d1 = v1 - v3;
    vec4 d2 = v2 - v3;

    float alpha = acos(dot(d1, d2) / (length(d1) * length(d2)));
    alpha = min(alpha, M_PI - alpha);
    float beta = M_PI/2 - alpha;

    return abs(cos(beta) * length(d1));
}

void main(void) {

    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;

    _EmitVertex(p0, dist(p0, p1, p2), 0, 0);
    _EmitVertex(p1, 0, dist(p1, p0, p2), 0);
    _EmitVertex(p2, 0, 0, dist(p2, p0, p1));

    EndPrimitive();
}
