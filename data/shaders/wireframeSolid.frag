#version 150 core

in float edge_dist[3];

out vec4 fragColor;

void main(void) {
    float dist = min(edge_dist[0], min(edge_dist[1], edge_dist[2]));

    float redFactor = 20;
    float greenFactor = 7;
    float blueFactor = 0;

    fragColor = vec4(dist * redFactor, dist * greenFactor, dist * blueFactor, 1.0);
}
