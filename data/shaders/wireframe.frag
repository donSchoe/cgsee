#version 150 core

in float edge_dist[3];

out vec4 fragColor;

void main(void) {
    float dist = min(edge_dist[0], min(edge_dist[1], edge_dist[2]));
    float edge = 0.015;

    if(dist > edge)
        discard;

    float minus = 0.2;
    float haloStep = dist / edge - minus;
    float haloColor = smoothstep(0, 1 - minus, max(0, haloStep));
    float coreGreen = 0.6;

    fragColor = vec4(haloColor, haloColor + coreGreen, haloColor, 1.0);
}
